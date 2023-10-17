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

#include "Archetype.hxx"
#include "CNonPhysical.hxx"
#include "EngineProtos.hxx"
#include "Defs.hxx"
#include "ILiving.hxx"
#include "CInstanceFactory.hxx"
#include "CProjectile.hxx"

/*
 * Class representing the world instance
 * of a piece of equipment. Behavior is dictated by
 * its Archetype.
 */
class CEquipInstance : public CNonPhysical, public virtual ILiving
{
public:
	enum
	{
		InstanceFlag = 1 << 10
	};

	struct CreationParameters
	{
		Archetype const *pArch;
		CEquippedObject *pParent;
	};

	CEquipInstance(void)
		: m_vChildSpawnOffset(0.0f, 0.0f)
	{
		//nothing to do
	};

	virtual ~CEquipInstance()
	{ };

	virtual void CEquipInstance::instance_create(InstanceId const instanceId)
	{
		this->m_iInstanceId = instanceId;

		this->m_uiInstanceFlags =
			ILiving::InstanceFlag |
			IWorldInstance::InstanceFlag |
			CNonPhysical::InstanceFlag |
			CEquipInstance::InstanceFlag;

		this->m_uiReferenceCount = 0;
		this->m_bMarkedForDeletion = false;
	}

	virtual void initialize(CreationParameters &creationParms)
	{
		this->m_pArch = creationParms.pArch;
		this->m_pParent = creationParms.pParent;
		this->m_bAlive = true;
	};

	virtual void alive_tick(float const flTick)
	{ };

	virtual void alive_set(bool const bAlive) final
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		this->m_bAlive = bAlive;
	};

	virtual bool is_alive(void) final
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_bAlive;
	};

	void set_child_spawn_offset(Vector2f const &vOffset)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		this->m_vChildSpawnOffset = vOffset;
	};

	Vector2f get_child_spawn_offset(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_vChildSpawnOffset;
	};

	virtual Archetype const * get_archetype(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_pArch;
	};

protected:
	CEquippedObject  *m_pParent;
	Archetype const *m_pArch;
	bool m_bAlive;
	Vector2f m_vChildSpawnOffset;
};

/*
 * Represents a weapon instance.
 * 
 * Has functions for determining if the weapon may be fired,
 * and of course for firing it.
 */
class CWeaponInstance : public CEquipInstance
{
public:
	enum
	{
		InstanceFlag = 1 << 11
	};

	CWeaponInstance()
		: m_flRefireTimeRemaining(-1.0f)
	{
	};

	virtual ~CWeaponInstance()
	{ };

	virtual void CWeaponInstance::instance_create(InstanceId const instanceId) final
	{
		this->m_iInstanceId = instanceId;

		this->m_uiInstanceFlags =
			ILiving::InstanceFlag |
			IWorldInstance::InstanceFlag |
			CNonPhysical::InstanceFlag |
			CEquipInstance::InstanceFlag |
			CWeaponInstance::InstanceFlag;

		this->m_uiReferenceCount = 0;
		this->m_bMarkedForDeletion = false;
	}

	virtual void alive_tick(float const flTick) final
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		this->m_flRefireTimeRemaining -= flTick;
	};

	bool ready_to_fire(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);
		return this->m_bAlive && m_flRefireTimeRemaining <= 0.0f;
		
	};

	float get_refire_time_remaining(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);
		return this->m_flRefireTimeRemaining;
	};

	void fire(void);

private:
	INLINE WeaponArch const * get_weapon_arch()
	{
		return reinterpret_cast<WeaponArch const *>(this->m_pArch);
	};

	void reset_fire_time(void)
	{
		this->m_flRefireTimeRemaining = this->get_weapon_arch()->flRefireDelay;
	};

	float m_flRefireTimeRemaining;
};