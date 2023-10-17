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
#include "CEquipInstance.hxx"
#include "CEquippedObject.hxx"

/*
 * The weapon fires.
 * 
 * A projectile is created, whose instance is sent to the world.
 */
void CWeaponInstance::fire(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->reset_fire_time();

	//Create the projectile
	InstanceId uiProjectileId;
	CProjectile * pProjectile;

	CInstanceFactory::create<CProjectile>(uiProjectileId, pProjectile);

	WeaponArch const *pWeapArch = this->get_weapon_arch();

	CProjectile::CreationParameters parms;
	float fParentRotation = this->m_pParent->get_rotation();
	Vector2f vInitialPosition = this->m_pParent->get_position();
	Matrix2f mRotationMatrix = Matrix2f::rotation_matrix(fParentRotation);
	Vector2f vInheritedVelocity = this->m_pParent->get_velocity();
	vInitialPosition += mRotationMatrix * this->m_vChildSpawnOffset;

	Vector2f vMuzzleVelocity = Vector2f(pWeapArch->flMuzzleVelocity * cos(deg2rad(fParentRotation)),
		pWeapArch->flMuzzleVelocity * sin(deg2rad(fParentRotation)));

	/*
	 * Computation for the projectile's damage multiplier.
	 * TODO: Make it less hacky, have some kind of inbuilt weapon type
	 * instead of simply checking energy cost = 0 to determine if it's
	 * kinetic or energy.
	 */

	ICharacterEntity* pOriginatingEntity = this->m_pParent->get_parent_entity();
	if (pOriginatingEntity == nullptr)
	{
		parms.flDamageMultiplier = 1.0f;
	}
	else
	{
		float flStat;

		//if less than a lower threshold,
		//assume it's a kinetic weapon
		if (pWeapArch->uiArchType == ARCH_MISSILE)
		{
			flStat = pOriginatingEntity->get_stat(Stat::MISSILE_PROFICIENCY);
		}
		else if (pWeapArch->flEnergyCost < 0.01f)
		{
			flStat = pOriginatingEntity->get_stat(Stat::KINETIC_PROFICIENCY);
		}
		else
		{
			flStat = pOriginatingEntity->get_stat(Stat::LASER_PROFICIENCY);
		}

		parms.flDamageMultiplier = flStat * 0.01f + 1.0f;
	}

	parms.vPosition = vInitialPosition;
	parms.flAngularVelocity = 0.0f;
	parms.flRotation = fParentRotation;
	parms.uiAppearanceId = pWeapArch->uiProjectileAppearance;
	parms.pWeapArch = pWeapArch;
	parms.bActivated = true;
	parms.iParentInstanceId = this->m_pParent->instance_get_id();
	parms.vVelocity = vMuzzleVelocity + vInheritedVelocity;
	parms.flMass = 0.1f;
	parms.flRadius = 5.0f;
	pProjectile->initialize(parms);

	SG::get_world()->instance_enqueue(pProjectile);

	SG::get_audio_manager()->play_sound_spatialized(pWeapArch->uiFireSound, vInitialPosition, 0.5f, 250.0f);
};