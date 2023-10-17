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
#include "CDestinedProjectile.hxx"

CDestinedProjectile::CDestinedProjectile(void)
{
	CProjectile::CProjectile();
}

CDestinedProjectile::~CDestinedProjectile(void)
{
	//nothing to do
}

void CDestinedProjectile::alive_tick(float const fDelta)
{
	this->m_mFieldAccess.lock();

	if(this->m_flProjTimeElapsed < this->m_flProjScriptTime)
	{
		this->m_flProjTimeElapsed += fDelta;

		if(this->m_pWeapArch->uiArchType == ARCH_MISSILE)
		{
			MissileArch const *pMissileArch = reinterpret_cast<MissileArch const *>(this->m_pWeapArch);

			if(this->m_flSmokeTimer < 0.0f)
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
		}
	}
	else
	{
		this->m_mFieldAccess.unlock();

		CProjectile::alive_tick(fDelta);

		return;
	}

	this->m_mFieldAccess.unlock();
}

void CDestinedProjectile::physics_tick(float const flDelta)
{
	this->m_mFieldAccess.lock();

	if(this->m_flProjTimeElapsed < this->m_flProjScriptTime)
	{
		Vector2f vNewOffset;
		float flNewRotation;

		this->m_fDestinyFunction(this->m_flProjTimeElapsed, vNewOffset, flNewRotation);

		this->m_vPosition = this->m_vProjOrigin + vNewOffset;
		this->m_flRotation = this->m_flProjRotationBase + flNewRotation;
	}
	else
	{
		this->m_mFieldAccess.unlock();

		CProjectile::physics_tick(flDelta);

		return;
	}

	this->m_mFieldAccess.unlock();
}

void CDestinedProjectile::initialize(CreationParameters const &creationParameters)
{
	this->m_vProjVelocityBase = creationParameters.vVelocity;
	this->m_vProjOrigin = creationParameters.vPosition;
	this->m_flProjRotationBase = creationParameters.flRotation;
	this->m_flProjScriptTime = creationParameters.flProjScriptTime;
	this->m_flProjTimeElapsed = 0.0f;
	this->m_fDestinyFunction = creationParameters.fDestinyFunction;

	CProjectile::initialize(creationParameters);

	if(!creationParameters.bHasLifetimeAfterDestiny)
	{
		this->m_fLifetimeRemaining = 0.0f;
	}
}