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
#pragma once

#include "EngineProtos.hxx"
#include "ILiving.hxx"
#include "CEquipInstance.hxx"
#include "CObject.hxx"
#include "CAttitudeSet.hxx"
#include "IInteractable.hxx"
#include "IStatusEffect.hxx"

/*
 * EquippedObject 
 */
class CEquippedObject : public CObject, public virtual ILiving, public IInteractable
{
public:
	enum
	{
		InstanceFlag = 1 << 8
	};

	struct CreationParameters : public CObject::CreationParameters
	{
		bool bActivated;
		Loadout pStartingLoadout;
		Faction const *pFaction;
		Pilot const *pPilot;
		float flCapacitorEnergy;
		float flCapacitorRegenRate;
		unsigned long ulExpValue;
		ICharacterEntity *pParentEntity;
	};

	CEquippedObject(void);
	virtual ~CEquippedObject(void);

	virtual unsigned long get_exp_value(void);

	virtual void instance_create(InstanceId const);
	
	void fire_weapons_if_ready(ArchType const);

	virtual float get_capacitor_energy(void);
	virtual void set_capacitor_energy(float const);
	virtual float get_max_capacitor_energy(void);

	//members inherited from ILiving
	virtual void alive_tick(float const);
	virtual void alive_set(bool const) final;

	virtual bool is_alive(void) final
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_bAlive;
	};

	virtual unsigned int get_physics_type(void) const
	{
		return PhysicsType::Collidable | PhysicsType::HasCallback;
	};

	virtual void initialize(CreationParameters const &);

	size_t get_children_count(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_lChildIds.size();
	};

	std::vector<InstanceId> get_children(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_lChildIds;
	};

	InstanceId get_ai_controller(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);
		
		return this->m_iAIInstanceId;
	};

	CAttitudeSet get_attitude_set(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_attitudeSet;
	};

	void set_attitude_set(CAttitudeSet const &attitudeSet)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		this->m_attitudeSet = attitudeSet;
	};

	float get_average_weapon_muzzle_velocity(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		float fMuzzleVelocitySum = 0.0f;
		int iNumWeapons = 0;

		for(ArchId const archId : this->m_pStartingLoadout)
		{
			Archetype const *arch = SG::get_game_data_manager()->get_arch<Archetype>(archId);

			if(arch == nullptr) continue;

			if(arch->uiArchType == ARCH_WEAPON)
			{
				fMuzzleVelocitySum += reinterpret_cast<WeaponArch const *>(arch)->flMuzzleVelocity;
				++iNumWeapons;
			}
		}

		if(!iNumWeapons) return 0.0f;

		return fMuzzleVelocitySum / iNumWeapons;
	};

	float get_max_weapon_range(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		float fHighestRange = 0.0f;
		int iNumWeapons = 0;

		for (ArchId const archId : this->m_pStartingLoadout)
		{
			Archetype const *arch = SG::get_game_data_manager()->get_arch<Archetype>(archId);

			if (arch->uiArchType == ARCH_WEAPON)
			{
				float flRange = reinterpret_cast<WeaponArch const *>(arch)->flMuzzleVelocity;
				if(flRange > fHighestRange)
				{
					fHighestRange = flRange;
				}
				++iNumWeapons;
			}
		}

		return fHighestRange;
	};

	Faction const *get_faction(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_pFaction;
	};

	virtual void interact(InstanceId const);

	std::list<IStatusEffect *> get_status_effects(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_vStatusEffects;
	};

	virtual void inflict_status(IStatusEffect *);

	virtual void perform_hostile_action(InstanceId const);

	virtual ICharacterEntity *get_parent_entity(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_pParentEntity;
	};

	void enable_weapons(bool const);

protected:
	bool m_bAlive;
	bool m_bWeaponsEnabled;
	std::vector<InstanceId> m_lChildIds;
	Loadout m_pStartingLoadout;
	Faction const *m_pFaction;
	Pilot const *m_pPilot;
	InstanceId m_iAIInstanceId;
	CAttitudeSet m_attitudeSet;
	std::list<IStatusEffect *> m_vStatusEffects;
	float m_flCapacitorEnergy;
	float m_flMaxCapacitorEnergy;
	float m_flCapacitorRegenRate;
	unsigned long m_ulExpValue;
	ICharacterEntity *m_pParentEntity;

	float get_hull_damage_multiplier(void) const;
	float get_shield_damage_multiplier(void) const;
	float get_hull_drain_percent(void) const;
	float get_shield_drain_percent(void) const;
	float get_capacitor_drain_percent(void) const;
	float get_engine_thrust_multiplier(void) const;

	/*
	 * Child instances should call this when destroyed, so child objects
	 * can be removed.
	 */
	void notify_deletion()
	{
		//Walk child instances
		for(InstanceId childId : this->m_lChildIds)
		{
			//Retrieve the world instance
			IWorldInstance *pChildInstance = SG::get_engine()->instance_get(childId);

			pChildInstance->mark_for_deletion();
		}

		//if we had an AI controller active, delete it
		if(this->m_iAIInstanceId != INVALID_INSTANCE)
		{
			IWorldInstance *pAIInstance = SG::get_engine()->instance_get(this->m_iAIInstanceId);

			pAIInstance->mark_for_deletion();
		}
	};
};