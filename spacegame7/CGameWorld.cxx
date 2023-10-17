/**
 * Copyright (C) 2020 Jacob Farnsworth.
 *
 * This file is part of the Spaced Out 4 project.
 *
 * Spaced Out 4 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, version 2.
 *
 * Spaced Out 4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "ILiving.hxx"
#include "IPhysicalObject.hxx"
#include "IWorldObject.hxx"
#include "CGameWorld.hxx"
#include "CShip.hxx"
#include <algorithm>

void CGameWorld::enqueue_event_callback(float const flTime, WorldEventCallback fCb)
{
	//std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	this->m_vEventTimers.push_back({flTime, fCb});
}

void CGameWorld::begin_world_transaction(void)
{
	this->m_mFieldAccess.lock();
}

void CGameWorld::end_world_transaction(void)
{
	this->m_mFieldAccess.unlock();
}

void CGameWorld::destroy_all_instances(void)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	for(auto pInstance : this->m_lInstances)
	{
		CInstanceFactory::destroy(pInstance->instance_get_id());
	}

	this->m_lInstances.clear();
}

void CGameWorld::set_player(InstanceId const iPlayerId)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	this->m_iPlayerId = iPlayerId;
}

InstanceId CGameWorld::get_player(void)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(!SG::get_engine()->instance_is_allocated(this->m_iPlayerId))
	{
		//on a lazy basis, reset the player ID if it is no longer valid

		this->m_iPlayerId = INVALID_INSTANCE;
	}

	return this->m_iPlayerId;
}

InstanceId CGameWorld::get_player_unchecked(void)
{
	if (!SG::get_engine()->instance_is_allocated(this->m_iPlayerId))
	{
		//on a lazy basis, reset the player ID if it is no longer valid

		this->m_iPlayerId = INVALID_INSTANCE;
	}

	return this->m_iPlayerId;
}

/*
 * enqueue an instance for addition to the instance list.
 * this function should be preferred over instance_add for
 * deadlock prevention purposes
 */
void CGameWorld::instance_enqueue(IWorldInstance *pInstance)
{
	std::lock_guard<Spinlock> lock(this->m_mQueueAccess);

	this->m_lInstanceQueue.push_back(pInstance);
}

/*
 * add an instance to this world, typically
 * one created via CInstanceFactory
 */
void CGameWorld::instance_add(IWorldInstance *pInstance)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	this->m_lInstances.push_back(pInstance);
}

/*
 * removes an instance from this world.
 * CInstanceFactory::destroy should be invoked on the instance
 * immediately after removal from the world
 */
void CGameWorld::instance_remove(IWorldInstance *pInstance)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);
    
    auto itInstancesEnd = this->m_lInstances.end();

	for(auto i = this->m_lInstances.begin(); i != itInstancesEnd; ++i)
	{
		if(*i == pInstance)
		{
			this->m_lInstances.erase(i);
			break;
		}
	}
}

/*
 * invoked once per tick by CGame.
 * 
 * flDelta is the time in seconds since the last tick
 */
void CGameWorld::world_tick(float const flDelta)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);


	bool bCbIncrement;

	/*
	 * Walking the callback timers
	 */
	for(auto i = this->m_vEventTimers.begin(); i != this->m_vEventTimers.end();  bCbIncrement ? ++i : i)
	{
		bCbIncrement = true;

		i->flTimeRemaining -= flDelta;
		
		if(i->flTimeRemaining <= 0.0f)
		{
			i->fCallback(this);
			this->m_vEventTimers.erase(i++);
			bCbIncrement = false;
		}
	}

	/*
	* If there are any enqueued instances, we want to add them
	* to the primary instance list so that they can be actually
	* in the world
	*/
	this->m_mQueueAccess.lock();
	if(this->m_lInstanceQueue.size())
	{
		this->m_lInstances.splice(this->m_lInstances.end(), this->m_lInstanceQueue);
	}
	this->m_mQueueAccess.unlock();
    
    InstanceList::iterator itInstancesEnd = this->m_lInstances.end();
	InstanceList::value_type pInstance;
    
    /*
     * if an instance was erased from the list, this should be
     * set to false, otherwise, leave it true
     */
    bool bIncrement;

    /*
     * iterate instances
     */
	for(auto i = this->m_lInstances.begin(); i != itInstancesEnd; bIncrement?++i:i)
	{
		pInstance = *i;
        bIncrement = true;
		InstanceFlags uiFlags = pInstance->instance_get_flags();

		if(uiFlags & IWorldObject::InstanceFlag)
		{
			IWorldObject *pObject = static_cast<IWorldObject*>(pInstance);

			/*
			 * if the object has not been initialized, skip it
			 */
			if(!pObject->is_initialized())
			{
				continue;
			}

			//otherwise, attempt to resolve collisions
			if(uiFlags & IPhysicalObject::InstanceFlag)
			{
				this->detect_and_resolve_collisions(pObject);
			}
		}
        
		/*
		 * we would like to destroy instances marked for deletion,
		 * however, we should ensure that the instance can be
		 * safely destroyed (has no references, etc)
		 */
        if(pInstance->is_marked_for_deletion())
        {
            InstanceId iId = pInstance->instance_get_id();
            
            //don't call instance_remove(), erasing manually is
            //faster since we already have an iterator, and we
            //avoid a potential deadlock
            this->m_lInstances.erase(i++);
            bIncrement = false;
            
            CInstanceFactory::destroy(iId);
            
            //continue iteration
            continue;
        }
        
        //invoke physics_tick() for implementers of IPhysicalObject
		if(uiFlags & IPhysicalObject::InstanceFlag)
		{
			//previously did a slower dynamic_cast here
			IPhysicalObject *pPhysicalObject = static_cast<IWorldObject*>(pInstance)->get_physical();

			if(pPhysicalObject)
			{
				pPhysicalObject->physics_tick(flDelta);
			}
		}
		
		//invoke alive_tick() for implementers of ILiving
		if(uiFlags & ILiving::InstanceFlag)
        {
			ILiving *pLiving = dynamic_cast<ILiving*>(pInstance);
            
			if(pLiving)
			{
				pLiving->alive_tick(flDelta);
			}
        }
	}
}

/*
 * Given a character description, create a ship object
 * in the world for that character.
 */
InstanceId CGameWorld::instantiate_character(Character const *pCharacter, Vector2f const &vPosition, float const flRotation)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	CShip *obj;
	InstanceId id;
	CInstanceFactory::create<CShip>(id, obj);

	//TODO: faction
	CShip::CreationParameters parms;

	if(pCharacter->nameColl != 0)
	{
		parms.szName = SG::get_game_data_manager()->get_random_name(pCharacter->nameColl);
	}
	else
	{
		parms.szName = pCharacter->name;
	}

	if(pCharacter->entityId)
	{
		ICharacterEntityManager *pCEM = SG::get_intransient_data_manager()->get_character_entity_manager();
		ICharacterEntity *pEntity = pCEM->get_character_entity(pCharacter->entityId);
		parms.pParentEntity = pEntity;
	}
	else
	{
		parms.pParentEntity = nullptr;
	}

	parms.szRadarClass = pCharacter->class_type;
	parms.vPosition = vPosition;
	parms.vVelocity = Vector2f(0.0f, 0.0f);
	parms.flRotation = flRotation;
	parms.flAngularVelocity = 0.0f;
	parms.pArch = SG::get_game_data_manager()->get_arch<ShipArch>(pCharacter->shipArchId);
	parms.uiAppearanceId = parms.pArch->uiShipAppearance;
	parms.flHitPoints = parms.pArch->flMaxHitPoints;
	parms.flRadius = 64.0f;
	parms.pStartingLoadout = *SG::get_game_data_manager()->get_loadout(pCharacter->loadoutId);
	parms.flHitPoints = parms.pArch->flMaxHitPoints;
	parms.flShieldPoints = parms.pArch->flMaxShieldPoints;
	parms.pFaction = SG::get_game_data_manager()->get_faction(pCharacter->factionId);
	parms.pPilot = SG::get_game_data_manager()->get_pilot(pCharacter->pilotId);
	parms.flRotationalInertia = parms.pArch->flRotationalInertia;
	parms.bActivated = true;
	parms.bIsPlayer = false;
	parms.uiInvulnerable = pCharacter->keyCharacter;
	parms.uiDropTable = pCharacter->dropTableId;
	parms.flCapacitorEnergy = parms.pArch->flCapacitorEnergy;
	parms.flCapacitorRegenRate = parms.pArch->flCapacitorRegenRate;
	parms.ulExpValue = pCharacter->ulExpValue;
	obj->initialize(parms);

	this->m_lInstances.push_back(obj);

	return id;
}

/*
 * This function is exposed to let instances within the world thread
 * (e.g. AI controllers) get a peek at other instances.
 * Should never be called from outside the world thread. Instead,
 * use the instance walking functions.
 */
void CGameWorld::get_all_instances(std::list<InstanceId> &vInstances)
{
	for(IWorldInstance *obj : this->m_lInstances)
	{
		vInstances.push_back(obj->instance_get_id());
	}
}

InstanceId CGameWorld::get_instance_at(Vector2f const &vTargetPosition)
{
	InstanceList::iterator itInstancesEnd = this->m_lInstances.end();
	InstanceList::value_type pInstance;

	/*
	* iterate instances
	*/
	for (auto i = this->m_lInstances.begin(); i != itInstancesEnd; ++i)
	{
		pInstance = *i;

		InstanceFlags uiFlags = pInstance->instance_get_flags();

		if (uiFlags & IWorldObject::InstanceFlag && uiFlags & IPhysicalObject::InstanceFlag)
		{
			/*
			* dynamic cast was replaced with get_physical(), which should always
			* succeed if an object has IPhysicalObject's instance flag
			*/
			IWorldObject *pOtherObject = static_cast<IWorldObject*>(pInstance);
			IPhysicalObject *pOtherObjectPhysical = pOtherObject->get_physical();

			unsigned int otherPhysType = pOtherObjectPhysical->get_physics_type();

			/*
			* if the other object is not collidable we would not like to continue
			* resolving the collision.
			*/
			if (!(otherPhysType & IPhysicalObject::PhysicsType::Collidable))
			{
				continue;
			}

			Vector2f vMyPosition = vTargetPosition;
			Vector2f vOtherPosition = pOtherObject->get_position();
			Vector2f vDistance = vMyPosition - vOtherPosition;
			float flOtherRadius = pOtherObjectPhysical->get_radius();
			float flDistance = vDistance.magnitude();

			//collision was detected, resolve it
			if (flDistance < flOtherRadius)
			{
				return pOtherObject->instance_get_id();
			}
		}
	}

	return INVALID_INSTANCE;
}

/*
 * invoked when the game is shifting to a new state.
 * in this case, we should clean the world of all entities
 */
void CGameWorld::shifting_out(void)
{
	this->m_mQueueAccess.lock();
	for (auto pInstance : this->m_lInstanceQueue)
	{
		CInstanceFactory::destroy(pInstance->instance_get_id());
	}
	this->m_lInstanceQueue.clear();
	this->m_mQueueAccess.unlock();

	this->m_mFieldAccess.lock();

	this->m_vEventTimers.clear();
	this->m_vStaticObjects.clear();

	this->m_iPlayerId = -1;
	for (auto pInstance : this->m_lInstances)
	{
		CInstanceFactory::destroy(pInstance->instance_get_id());
	}
	this->m_lInstances.clear();
	this->m_mFieldAccess.unlock();
}

void CGameWorld::create_static(StaticTextureId const iStaticTextureId,
	float const flPositionX, float const flPositionY, float const flPositionZ,
	float const flRotation,
	Vector2f const &vSize)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	this->m_vStaticObjects.emplace_back(CStaticObject(iStaticTextureId, flPositionX, flPositionY, flPositionZ, flRotation, vSize));

	/*
	 * Since we changed to use GL_ALPHA_TEST and depth testing concurrently,
	 * this is no longer necessary to manipulate draw order.
	 */
	// this->sort_statics();
}

std::vector<CStaticObject> const &CGameWorld::get_all_statics(void)
{
	this->m_mFieldAccess.lock();

	return this->m_vStaticObjects;
}

void CGameWorld::release_statics(void)
{
	this->m_mFieldAccess.unlock();
}

/*
 * an external class is requesting access to the world instances.
 * initialize an iterator with the beginning of the instance list,
 * then return the first instance.
 * 
 * If this is being called from a thread other than the world thread,
 * you MUST call world_begin_transaction() and world_end_transaction()
 * before and after the entire walk
 */
IWorldInstance * CGameWorld::instance_walk_begin()
{
	this->m_iInstanceWalkIterator = this->m_lInstances.begin();

	return this->instance_walk_next();
}

/*
 * an external class has requested the next world instance
 * in the chain. step the iterator and return the next world instance
 */
IWorldInstance * CGameWorld::instance_walk_next()
{
INSTANCE_WALK_TRY_AGAIN:
	if(this->m_iInstanceWalkIterator == this->m_lInstances.end())
	{
		return nullptr;
	}

	IWorldInstance *pInstance = *this->m_iInstanceWalkIterator++;

	InstanceFlags uiFlags = pInstance->instance_get_flags();

	if(uiFlags & IWorldObject::InstanceFlag)
	{
		IWorldObject *pObject = dynamic_cast<IWorldObject*>(pInstance);

		/*
		* if the object has not been initialized, skip it
		*/
		if (!pObject->is_initialized())
		{
			goto INSTANCE_WALK_TRY_AGAIN;
		}
	}

	return pInstance;
}

/*
 * since our physics model is incredibly simple (all objects are circular)
 * our collision-detection code can fit into one straightforward function
 */
void CGameWorld::detect_and_resolve_collisions(IWorldObject *pObject)
{
	//we already know pObject is a PhysicalObject
	IPhysicalObject *pObjectPhysical = dynamic_cast<IPhysicalObject*>(pObject);

	unsigned int physType = pObjectPhysical->get_physics_type();

	//skip this object if it is not collidable
	if(!(physType & IPhysicalObject::PhysicsType::Collidable))
	{
		return;
	}

	InstanceList::iterator itInstancesEnd = this->m_lInstances.end();
	InstanceList::value_type pInstance;

	/*
	* iterate instances
	*/
	for(auto i = this->m_lInstances.begin(); i != itInstancesEnd; ++i)
	{
		pInstance = *i;

		if(pInstance == pObject) continue;

		InstanceFlags uiFlags = pInstance->instance_get_flags();

		if(uiFlags & IWorldObject::InstanceFlag && uiFlags & IPhysicalObject::InstanceFlag)
		{
			/*
			 * dynamic cast was replaced with get_physical(), which should always
			 * succeed if an object has IPhysicalObject's instance flag
			 */
			IWorldObject *pOtherObject = static_cast<IWorldObject*>(pInstance);
			IPhysicalObject *pOtherObjectPhysical = pOtherObject->get_physical();

			unsigned int otherPhysType = pOtherObjectPhysical->get_physics_type();

			/*
			 * if the other object is not collidable we would not like to continue
			 * resolving the collision.
			 */
			if(!(otherPhysType & IPhysicalObject::PhysicsType::Collidable))
			{
				continue;
			}

			Vector2f vMyPosition = pObject->get_position();
			Vector2f vOtherPosition = pOtherObject->get_position();
			Vector2f vDistance = vMyPosition - vOtherPosition;
			float flMyRadius = pObjectPhysical->get_radius();
			float flOtherRadius = pOtherObjectPhysical->get_radius();
			float flRadii = flMyRadius + flOtherRadius;
			float flDistance = vDistance.magnitude();

			//collision was detected, resolve it
			if(flDistance < flRadii)
			{
				//no fucking idea why I was using this previous formula dividing by distance -
				//seems to work for normal collisions between two solid bodies, but collisions with immovable
				//objects result in small distance values for some reason causing bodies to get shot
				//far distances
				//float flResolutionVectorMagnitude = (flDistance - flRadii) / flDistance;

				float flResolutionVectorMagnitude = flDistance - flRadii;

				Vector2f vResolution = flResolutionVectorMagnitude * vDistance.normalize();

				//since verlet operates on position and acceleration,
				//adjusting position without adjusting internal "old position"
				//will give the objects' some velocity. whether this response
				//is physically accurate, I don't know, but it seems accurate enough,
				//so (for now) we only resolve here and don't add any impulses
				//to the mix
				if((physType & IPhysicalObject::PhysicsType::Solid)
					&& (otherPhysType & IPhysicalObject::PhysicsType::Solid))
				{
					pObject->set_position(vMyPosition - vResolution / 0.5f);
					pOtherObject->set_position(vOtherPosition + vResolution / 0.5f);
				}
				else if((physType & IPhysicalObject::PhysicsType::Solid)
					&& (otherPhysType & IPhysicalObject::PhysicsType::Immovable))
				{
					pObject->set_position(vMyPosition - vResolution);
				}
				else if((physType & IPhysicalObject::PhysicsType::Immovable)
					&& (otherPhysType & IPhysicalObject::PhysicsType::Solid))
				{
					pOtherObject->set_position(vOtherPosition + vResolution);
				}

				//invoke collision callback for both objects
				if(physType & IPhysicalObject::PhysicsType::HasCallback)
				{
					pObjectPhysical->collision_callback(pOtherObject);
				}

				if(otherPhysType & IPhysicalObject::PhysicsType::HasCallback)
				{
					pOtherObjectPhysical->collision_callback(pObject);
				}
			}
		}
	}
}

void CGameWorld::sort_statics(void)
{
	std::sort(this->m_vStaticObjects.begin(), this->m_vStaticObjects.end(), [](CStaticObject& a, CStaticObject& b)
		{
			float ax, ay, az;
			float bx, by, bz;

			a.get_position(ax, ay, az);
			b.get_position(bx, by, bz);

			return az < bz;
		});
}