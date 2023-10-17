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
#include "CProjectile.hxx"
#include "CShip.hxx"

CProjectile::CProjectile(void)
{
	CObject::CObject();
}

CProjectile::~CProjectile(void)
{
	//nothing to do
}

void CProjectile::alive_tick(float const fDelta)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_fLifetimeRemaining -= fDelta;

	if(this->m_fLifetimeRemaining < 0.0f)
	{
		//the projectile has expired, disable it and mark it for deletion (or explode if it's a missile)

		if(this->is_missile())
		{
			this->explode_missile();
		}
		else
		{
			this->m_bAlive = false;
			this->m_bMarkedForDeletion = true;
		}

		return;
	}

	if(this->m_pWeapArch->uiArchType == ARCH_MISSILE)
	{
		MissileArch const *pMissileArch = reinterpret_cast<MissileArch const *>(this->m_pWeapArch);

		if(this->m_fTimeToAcquireSeekTarget < 0.0f && !this->m_bTargetAcquired)
		{
			if (this->m_iSeekTarget == INVALID_INSTANCE)
			{
				CShip *pParentObj = SG::get_engine()->instance_get_checked<CShip>(this->m_iParentInstanceId);

				if (pParentObj != nullptr)
				{
					this->m_iSeekTarget = pParentObj->get_target();
				}
			}

			//even if we didn't really acquire a target, mark that we tried
			//otherwise the missile will try to reacquire newly selected
			//targets
			this->m_bTargetAcquired = true;
		}
		else
		{
			this->m_fTimeToAcquireSeekTarget -= fDelta;
		}
 
		if (this->m_iSeekTarget != INVALID_INSTANCE)
		{
			if (SG::get_engine()->instance_is_allocated(this->m_iSeekTarget))
			{
				IWorldObject *pTargetObj = static_cast<IWorldObject*>(SG::get_engine()->instance_get(this->m_iSeekTarget));
				IPhysicalObject *pTargetPhysics = pTargetObj->get_physical();

				Vector2f vParentPosition = this->m_vPosition;
				Vector2f vTargetPosition = pTargetObj->get_position();
				Vector2f vParentVelocity = this->m_vVelocity;
				Vector2f vTargetVelocity = Vector2f(0.0f, 0.0f);
				float flParentRotation = this->m_flRotation;

				if (pTargetPhysics)
				{
					vTargetVelocity = pTargetPhysics->get_velocity();
				}

				Vector2f vPositionDelta = vTargetPosition - vParentPosition;
				Vector2f vVelocityDelta = vTargetVelocity - vTargetVelocity;

				float flTravelDistance = vPositionDelta.magnitude();
				//float flAverageTravelTime = clamp(flTravelDistance / vParentVelocity.magnitude(), 0.0f, this->m_fSeekTime / 4.0f);
				//float flAverageTravelTime = clamp(flTravelDistance / vParentVelocity.magnitude(), 0.0f, this->m_fSeekTime);
				float flAverageTravelTime = sqrt(2.0f * flTravelDistance / pMissileArch->flMotorAcceleration);

				//float flTryhardSeekRatio = clamp(1.5f - m_fSeekTimeRemaining / m_fSeekTime, 0.0f, 1.0f);
				float flTryhardSeekRatio = clamp(1.0f - (vParentVelocity.normalize()).dot(vPositionDelta.normalize()), 0.5f, 1.0f);
				Vector2f vPredictedTargetPosition = vTargetPosition + flTryhardSeekRatio * ((flAverageTravelTime * vTargetVelocity) - (flAverageTravelTime * vParentVelocity));

				this->m_flRotation = rad2deg(look_at_angle(this->m_vPosition, vPredictedTargetPosition));
			}
			else
			{
				this->m_iSeekTarget = INVALID_INSTANCE;
			}
		}

		if (this->m_fTimeToAcquireSeekTarget < 0.0f)
		{
			if (this->m_fSeekTimeRemaining < 0.0f)
			{
				this->m_vAcceleration = Vector2f(0.0f, 0.0f);
			}
			else
			{
				if (this->m_flSmokeTimer < 0.0f)
				{
					std::uniform_real_distribution<float> dist_rot(-180.0f, 180.0f);
					float flRotation = dist_rot(SG::get_random());

					SG::get_particle_manager()->add_particle(pMissileArch->uiMotorParticle, this->m_vPosition, Vector2f(0.0f, 0.0f), flRotation, 0.0f);

					this->m_flSmokeTimer = 0.005f;
				}
				else
				{
					this->m_flSmokeTimer -= fDelta;
				}

				this->m_vAcceleration = Matrix2f::rotation_matrix(this->m_flRotation) * Vector2f(pMissileArch->flMotorAcceleration, 0.0f);

				this->m_fSeekTimeRemaining -= fDelta;
			}
		}
	}
}

void CProjectile::alive_set(bool const bAlive)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_bAlive = bAlive;
}

/*
 * Initialize a new projectile object.
 * 
 * A pointer to the parent weapon's arch must be provided,
 * as well as the initial alive value, and the instance ID
 * of the object which fired this projectile.
 */
void CProjectile::initialize(CreationParameters const &creationParameters)
{
	this->m_bAlive = creationParameters.bActivated;
	this->m_fLifetimeRemaining = creationParameters.pWeapArch->flProjectileLifetime;
	this->m_iParentInstanceId = creationParameters.iParentInstanceId;
	this->m_pWeapArch = creationParameters.pWeapArch;
	this->m_flDamageMultiplier = creationParameters.flDamageMultiplier;
	this->m_bDealtDamage = false;

	//are we a missile?
	if(this->m_pWeapArch->uiArchType == ARCH_MISSILE)
	{
		MissileArch const *pMissileArch = reinterpret_cast<MissileArch const *>(this->m_pWeapArch);

		this->m_fTimeToAcquireSeekTarget = pMissileArch->flMotorDelay;
		this->m_fSeekTimeRemaining = pMissileArch->flMotorLifetime;
		this->m_fSeekTime = pMissileArch->flMotorLifetime;
		this->m_flSmokeTimer = -1.0f;
	}

	this->m_iSeekTarget = INVALID_INSTANCE;
	this->m_bTargetAcquired = false;

	CObject::initialize(creationParameters);
}

void CProjectile::collision_callback(IWorldObject *pCollidingObject)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(this->m_bAlive)
	{
		if(!(pCollidingObject->instance_get_flags() & CEquippedObject::InstanceFlag))
		{
			return;
		}

		if(pCollidingObject->instance_get_id() != this->m_iParentInstanceId)
		{
			this->explode_missile();
		}
	}
}

WeaponArch const * CProjectile::get_weapon_arch(void) const
{
	return this->m_pWeapArch;
}

void CProjectile::explode_missile(void)
{
	//disable the projectile and mark it for deletion

	this->m_bAlive = false;
	this->m_bMarkedForDeletion = true;

	if(this->m_pWeapArch->uiArchType == ARCH_MISSILE)
	{
		MissileArch const *pMissileArch = reinterpret_cast<MissileArch const *>(this->m_pWeapArch);

		SG::get_particle_manager()->add_particle(pMissileArch->uiMunitionExplosion, this->m_vPosition, Vector2f(0.0f, 0.0f), this->m_flRotation, 0.0f);

		SG::get_audio_manager()->play_sound_spatialized(4, this->m_vPosition, 1.0f, 250.0f);
	}
}