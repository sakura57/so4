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

#include "ILiving.hxx"
#include "CEquipInstance.hxx"
#include "CObject.hxx"

/*
* Projectile
*/
class CProjectile : public CObject, public virtual ILiving
{
public:
	enum
	{
		InstanceFlag = 1 << 12
	};

	struct CreationParameters : public CObject::CreationParameters
	{
		bool bActivated;
		WeaponArch const *pWeapArch;
		InstanceId iParentInstanceId;
		float flDamageMultiplier;
	};

	CProjectile(void);
	virtual ~CProjectile(void);

	virtual WeaponArch const * get_weapon_arch(void) const;

	//members inherited from ILiving
	virtual void alive_tick(float const);
	virtual void alive_set(bool const) final;

	virtual bool is_alive(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_bAlive;
	};

	virtual void instance_create(InstanceId const instanceId)
	{
		this->m_iInstanceId = instanceId;

		this->m_uiInstanceFlags =
			ILiving::InstanceFlag |
			IWorldInstance::InstanceFlag |
			IWorldObject::InstanceFlag |
			IPhysicalObject::InstanceFlag |
			CObject::InstanceFlag |
			CProjectile::InstanceFlag;

		this->m_uiReferenceCount = 0;
		this->m_bMarkedForDeletion = false;
	};

	virtual unsigned int get_physics_type(void) const
	{
		return PhysicsType::Collidable | PhysicsType::HasCallback;
	};

	virtual void initialize(CreationParameters const &);

	InstanceId get_parent_instance(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_iParentInstanceId;
	};

	void set_parent_instance(InstanceId const iParentInstanceId)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		this->m_iParentInstanceId = iParentInstanceId;
	};

	virtual void get_render_parms(RenderParameters &renderParms)
	{
		renderParms.vScale = Vector2f(1.0f, 1.0f);
		renderParms.fAlpha = 184;
	};

	virtual void collision_callback(IWorldObject *);

	InstanceId get_seek_target(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_iSeekTarget;
	};

	void set_seek_target(InstanceId const iSeekTargetId)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		this->m_iSeekTarget = iSeekTargetId;
	};

	bool is_missile(void) const
	{
		return this->m_pWeapArch->uiArchType == ARCH_MISSILE;
	};

	void set_lifetime_remaining(float const flLifetimeRemaining)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		this->m_fLifetimeRemaining = flLifetimeRemaining;
	};

	void set_dealt_damage(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		this->m_bDealtDamage = true;
	};

	bool get_dealt_damage(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);
		
		return this->m_bDealtDamage;
	};

	float get_damage_multiplier(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_flDamageMultiplier;
	};
protected:
	bool m_bAlive;

	WeaponArch const *m_pWeapArch;
	InstanceId m_iParentInstanceId;
	InstanceId m_iSeekTarget;
	float m_fTimeToAcquireSeekTarget;
	float m_fSeekTimeRemaining;
	float m_flSmokeTimer;
	float m_fSeekTime;

	float m_flDamageMultiplier;

	bool m_bDealtDamage;

	bool m_bTargetAcquired;

	float m_fLifetimeRemaining;
private:
	void explode_missile(void);
};