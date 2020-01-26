#include "CAsteroidMineable.hxx"
#include "CShip.hxx"
#include "CAIController.hxx"
#include "AsteroidProspectPanel.hxx"

#define DEFAULT_BREAK_PROBABILITY 0.05f

CAsteroidMineable::CAsteroidMineable(void)
{
	CEquippedObject::CEquippedObject();
}

CAsteroidMineable::~CAsteroidMineable(void)
{
	//nothing to do
}

void CAsteroidMineable::instance_create(InstanceId const instanceId)
{
	this->m_iInstanceId = instanceId;

	this->m_uiInstanceFlags =
		IWorldInstance::InstanceFlag |
		IWorldObject::InstanceFlag |
		IPhysicalObject::InstanceFlag |
		CObject::InstanceFlag |
		CEquippedObject::InstanceFlag |
		CAsteroidMineable::InstanceFlag |
		IRadarVisible::InstanceFlag |
		ILiving::InstanceFlag;

	this->m_uiReferenceCount = 0;
	this->m_bMarkedForDeletion = false;
}

void CAsteroidMineable::instance_destroy(InstanceId const iInstanceId)
{
	CEquippedObject::instance_destroy(iInstanceId);
}

void CAsteroidMineable::initialize(CreationParameters const& creationParameters)
{
	this->m_szRadarClass = creationParameters.szRadarClass;
	this->m_szName = creationParameters.szName;

	this->m_bUsed = false;

	this->m_flBreakProbability = DEFAULT_BREAK_PROBABILITY;
	this->m_flDropProbability = creationParameters.flDropProbability;
	this->m_szMaterialSerial = creationParameters.szMaterialSerial;
	this->m_uiMinQuantity = creationParameters.uiMinQuantity;
	this->m_uiMaxQuantity = creationParameters.uiMaxQuantity;
	this->m_szMaterialSerial = creationParameters.szMaterialSerial;
	this->m_uiAstfieldId = creationParameters.uiAstfieldId;

	CEquippedObject::initialize(creationParameters);
}

void CAsteroidMineable::alive_tick(float const flDelta)
{
	this->m_mFieldAccess.lock();

	if(this->m_bUsed == true)
	{
		CShip* pInteractorShip = SG::get_engine()->instance_get_checked<CShip>(this->m_iInteractorId);

		if(pInteractorShip)
		{
			pInteractorShip->set_velocity(Vector2f(0.0f, 0.0f));

			CAIController::aim_at_point(pInteractorShip, this->m_vPosition);
		}
	}

	this->m_mFieldAccess.unlock();

	CEquippedObject::alive_tick(flDelta);
}

void CAsteroidMineable::physics_tick(float const flDelta)
{
	this->m_mFieldAccess.lock();

	this->m_mFieldAccess.unlock();

	//invoke the base physics_tick
	CObject::physics_tick(flDelta);
}

void CAsteroidMineable::interact(InstanceId const interactor)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(this->m_bUsed == true)
	{
		return;
	}

	//Ensure the interactor is within range,
	//if not and it's the player, send a comm
	if(interactor != INVALID_INSTANCE)
	{
		CShip* pInteractorShip = SG::get_engine()->instance_get_checked<CShip>(interactor);

		if(!pInteractorShip->is_player())
		{
			return;
		}

		if(this->m_vPosition.distance(pInteractorShip->get_position()) < 500.0f)
		{
			SG::get_game_state_manager()->get_game_state()->state_send_notification("Analyzing...");

			SG::get_audio_manager()->play_sound(32);

			SG::get_interface_manager()->add_panel(new AsteroidProspectPanel(this->m_uiAstfieldId));
		}
		else
		{
			SG::get_game_state_manager()->get_game_state()->state_send_notification("Too far to interact");
		}
	}
}

void CAsteroidMineable::mine_asteroid()
{
	this->m_bAlive = false;

	//notify the EquippedObject of deletion,
	//so child equipment can be destroyed
	this->notify_deletion();

	//give the player their hard-earned materials
	if(this->m_szMaterialSerial.size())
	{
		ICharacterEntity* pPlayerEntity = SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity();
		IMaterialBank* pPlayerMatBank = pPlayerEntity->get_material_bank();
		
		std::uniform_int_distribution<unsigned int> quantityDist(this->m_uiMinQuantity, this->m_uiMaxQuantity);
		unsigned int uiQuantityToGive = quantityDist(SG::get_random());

		pPlayerMatBank->add_material(this->m_szMaterialSerial, uiQuantityToGive);
	}

	Matrix2f mRotation = Matrix2f::rotation_matrix(this->m_flRotation + 90.0f);
	Matrix2f mInvert = Matrix2f::rotation_matrix(180.0f);

	Vector2f vVelocityEstimate = this->estimate_velocity();

	std::uniform_real_distribution<float> dist_rot(-0.01f, 0.01f);
	std::uniform_real_distribution<float> dist_vel(-5.0f, -1.0f);

	std::mt19937 &random = SG::get_random();

	Vector2f vFinalVelocityPiece1 = vVelocityEstimate + (mRotation * Vector2f(dist_vel(random), 0.0f));
	Vector2f vFinalVelocityPiece2 = mInvert * vFinalVelocityPiece1;

	SG::get_particle_manager()->add_particle(16, this->m_vPosition, vFinalVelocityPiece1, this->m_flRotation, dist_rot(random));
	SG::get_particle_manager()->add_particle(17, this->m_vPosition, vFinalVelocityPiece2, this->m_flRotation, dist_rot(random));

	SG::get_audio_manager()->play_sound_spatialized(31, this->m_vPosition, 1.0f, 500.0f);

	SG::get_game_state_manager()->get_game_state()->state_send_notification("Resources acquired");

	this->m_bMarkedForDeletion = true;
}

void CAsteroidMineable::collision_callback(IWorldObject* pOtherObject)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(this->m_bMarkedForDeletion || !this->m_bAlive)
	{
		return;
	}

	//did we just collide with a projectile?
	if(pOtherObject->instance_get_flags() & CProjectile::InstanceFlag)
	{
		CProjectile* pProjectile = static_cast<CProjectile*>(pOtherObject);
		InstanceId uiProjParentId = pProjectile->get_parent_instance();

		//only mining laser can mine asteroids
		if(pProjectile->get_weapon_arch()->uiArchId == 38U)
		{
			if(!pProjectile->get_dealt_damage() && uiProjParentId != this->m_iInstanceId && uiProjParentId == SG::get_world()->get_player_unchecked())
			{
				if(SG::get_engine()->instance_is_allocated(uiProjParentId))
				{
					std::uniform_real_distribution<float> dropDist(0.0f, 1.0f);

					float flDropRoll = dropDist(SG::get_random());

					if(flDropRoll < this->m_flBreakProbability)
					{
						this->mine_asteroid();
					}
				}
			}
		}
	}
}