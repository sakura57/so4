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
#include "CEquippedObject.hxx"
#include "CEquipInstanceFactory.hxx"
#include "CAIController.hxx"


CEquippedObject::CEquippedObject(void)
{
	CObject::CObject();
}


CEquippedObject::~CEquippedObject(void)
{
	//nothing to do
}

unsigned long CEquippedObject::get_exp_value(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_ulExpValue;
}

void CEquippedObject::interact(InstanceId const interactor)
{
	SG::get_game_state_manager()->get_game_state()->state_send_notification("Can\'t interact");
}

void CEquippedObject::inflict_status(IStatusEffect *pStatus)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vStatusEffects.push_back(pStatus);
}

void CEquippedObject::instance_create(InstanceId const instanceId)
{
	this->m_iInstanceId = instanceId;

	this->m_uiInstanceFlags =
		IWorldInstance::InstanceFlag |
		IWorldObject::InstanceFlag |
		IPhysicalObject::InstanceFlag |
		CObject::InstanceFlag |
		CEquippedObject::InstanceFlag |
		ILiving::InstanceFlag;

	this->m_uiReferenceCount = 0;
	this->m_bMarkedForDeletion = false;
}

/*
 * This function will enumerate child instances. If it finds child instances
 * which are weapons, their refire timers are checked, then fired if they are ready.
 * 
 * Weapon instances implement ILiving, so their refire timers are decremented automatically
 * by the world thread.
 */
void CEquippedObject::fire_weapons_if_ready(ArchType const equip_type)
{
	if(this->m_bWeaponsEnabled == false)
	{
		return;
	}

	//Walk child instances
	for(InstanceId childId : this->m_lChildIds)
	{
		if(!SG::get_engine()->instance_is_allocated(childId))
		{
			continue;
		}

		//Retrieve the world instance
		IWorldInstance *childInstance = SG::get_engine()->instance_get(childId);

		//Determine if the child is a weapon
		if(childInstance->instance_get_flags() & CWeaponInstance::InstanceFlag)
		{
			//Get the child as a CWeaponInstance
			CWeaponInstance *weapInstance = static_cast<CWeaponInstance*>(childInstance);

			if (weapInstance->get_archetype()->uiArchType != equip_type) continue;

			WeaponArch const *pArch = reinterpret_cast<WeaponArch const *>(weapInstance->get_archetype());

			if(pArch && pArch->flEnergyCost <= this->m_flCapacitorEnergy)
			{
				//Fire if ready
				if(weapInstance->ready_to_fire())
				{
					this->m_flCapacitorEnergy -= pArch->flEnergyCost;

					weapInstance->fire();
				}
			}
		}
	}
}

void CEquippedObject::initialize(CreationParameters const &creationParameters)
{
	this->m_bAlive = creationParameters.bActivated;

	Loadout const &loadout = creationParameters.pStartingLoadout;


	//ok, now we need to spawn objects corresponding to each item in the
	//EquippedObj's loadout
	for(ArchId archId : loadout)
	{
		if(!archId) continue;

		this->m_lChildIds.push_back(CEquipInstanceFactory::create(this, archId));
	}

	this->m_pStartingLoadout = loadout;
	
	this->m_pPilot = creationParameters.pPilot;
	this->m_pFaction = creationParameters.pFaction;
	this->m_flMaxCapacitorEnergy = creationParameters.flCapacitorEnergy;
	this->m_flCapacitorEnergy = creationParameters.flCapacitorEnergy;
	this->m_flCapacitorRegenRate = creationParameters.flCapacitorRegenRate;
	this->m_ulExpValue = creationParameters.ulExpValue;
	this->m_pParentEntity = creationParameters.pParentEntity;
	this->m_bWeaponsEnabled = true;

	//if we don't have a null pilot, spawn an AI controller to control
	//the ship
	if(this->m_pPilot != nullptr)
	{
		CAIController *pAIController;

		CInstanceFactory::create<CAIController>(this->m_iAIInstanceId, pAIController);

		CAIController::CreationParameters parms;
		parms.pPilot = this->m_pPilot;
		parms.iParent = this->m_iInstanceId;
		parms.pParentEntity = this->m_pParentEntity;

		pAIController->initialize(parms);
		pAIController->alive_set(true);

		SG::get_world()->instance_enqueue(pAIController);
	}
	else
	{
		this->m_iAIInstanceId = INVALID_INSTANCE;
	}

	//initialize the object's attitude set
	this->m_attitudeSet.initialize(this->m_pFaction);

	CObject::initialize(creationParameters);
}

void CEquippedObject::alive_set(bool const bAlive)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_bAlive = bAlive;
}

void CEquippedObject::alive_tick(float const flDelta)
{
	SCOPE_LOCK(this->m_mFieldAccess);
	
	//replenish capacitor energy
	{
		float flCapacitorDelta = 0.0f;

		//start with capacitor regen
		flCapacitorDelta = this->m_flCapacitorRegenRate * flDelta;

		//account for capacitor drain
		flCapacitorDelta -= this->get_capacitor_drain_percent() * this->m_flMaxCapacitorEnergy * flDelta;

		float flNewCapacitorEnergy = clamp(this->m_flCapacitorEnergy + flCapacitorDelta, 0.0f, this->m_flMaxCapacitorEnergy);

		this->m_flCapacitorEnergy = flNewCapacitorEnergy;
	}

	//if we have any attitudes against objects that don't exist
	//any more, clean them up
	this->m_attitudeSet.cleanup_attitudes();

	//tick status effects and clean them up if any have expired
	for(auto i = this->m_vStatusEffects.begin(); i != this->m_vStatusEffects.end(); ++i)
	{
		(*i)->tick(flDelta);

		if((*i)->expired())
		{
			delete *i;

			//TODO: make this less shitty
			this->m_vStatusEffects.erase(i);

			break;
		}
	}
}

float CEquippedObject::get_capacitor_energy(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_flCapacitorEnergy;
}

float CEquippedObject::get_max_capacitor_energy(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_flMaxCapacitorEnergy;
}

void CEquippedObject::set_capacitor_energy(float const flCapacitorEnergy)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flCapacitorEnergy = flCapacitorEnergy;
}

float CEquippedObject::get_hull_damage_multiplier(void) const
{
	float multiplier = 1.0f;

	if (this->m_pParentEntity != nullptr)
	{
		multiplier -= this->m_pParentEntity->get_stat(Stat::HULL_INTEGRITY) * 0.01f;
	}

	for(IStatusEffect *pStatus : this->m_vStatusEffects)
	{
		multiplier *= pStatus->get_hull_damage_multiplier();
	}

	if (multiplier < 0.0f)
	{
		multiplier = 0.0f;
	}

	return multiplier;
}

float CEquippedObject::get_engine_thrust_multiplier(void) const
{
	float multiplier = 1.0f;

	if (this->m_pParentEntity != nullptr)
	{
		multiplier += this->m_pParentEntity->get_stat(Stat::EVASION) * 0.01f;
	}

	for(IStatusEffect *pStatus : this->m_vStatusEffects)
	{
		multiplier *= pStatus->get_engine_thrust_multiplier();
	}

	return multiplier;
}

float CEquippedObject::get_shield_damage_multiplier(void) const
{
	float multiplier = 1.0f;

	if (this->m_pParentEntity != nullptr)
	{
		multiplier -= this->m_pParentEntity->get_stat(Stat::SHIELD_KNOWLEDGE) * 0.01f;
	}

	for(IStatusEffect *pStatus : this->m_vStatusEffects)
	{
		multiplier *= pStatus->get_shield_damage_multiplier();
	}

	if (multiplier < 0.0f)
	{
		multiplier = 0.0f;
	}

	return multiplier;
}

float CEquippedObject::get_hull_drain_percent(void) const
{
	float multiplier = 0.0f;

	for(IStatusEffect *pStatus : this->m_vStatusEffects)
	{
		multiplier += pStatus->get_hull_drain_percent();
	}

	return multiplier;
}

float CEquippedObject::get_shield_drain_percent(void) const
{
	float multiplier = 0.0f;

	for(IStatusEffect *pStatus : this->m_vStatusEffects)
	{
		multiplier += pStatus->get_shield_drain_percent();
	}

	return multiplier;
}

float CEquippedObject::get_capacitor_drain_percent(void) const
{
	float multiplier = 0.0f;

	for(IStatusEffect *pStatus : this->m_vStatusEffects)
	{
		multiplier += pStatus->get_capacitor_drain_percent();
	}

	return multiplier;
}

void CEquippedObject::perform_hostile_action(InstanceId const iAggressor)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_attitudeSet.set_attitude(iAggressor, ATTITUDE_LOATHING);
}

void CEquippedObject::enable_weapons(bool const bEnabled)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_bWeaponsEnabled = bEnabled;
}