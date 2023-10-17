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
#include "CSpellKineticBarrageWeak.hxx"
#include "SGLib.hxx"
#include "CShip.hxx"
#include "CDestinedProjectile.hxx"


/*
	parms.fDestinyFunction = [iCaster, vInitialPosition](float const flAlpha, Vector2f &vNewPosition, float &flNewRotation)
	{
	CShip *pCaster = SG::get_engine()->instance_get_checked<CShip>(iCaster);

	Vector2f vCurrentOriginPosition = pCaster->get_position();

	//vNewPosition.x = log(flAlpha + 1.0f) * 150.0f * cos(5.0f * sqrt(flAlpha));
	//vNewPosition.y = log(flAlpha + 1.0f) * 150.0f * sin(5.0f * sqrt(flAlpha));
	vNewPosition.x = (1.0f - 1.0f / (flAlpha * 2.5f + 1.0f)) * 200.0f * cos(6.5f * sqrt(flAlpha)) + vCurrentOriginPosition.x - vInitialPosition.x;
	vNewPosition.y = (1.0f - 1.0f / (flAlpha * 2.5f + 1.0f)) * 200.0f * sin(6.5f * sqrt(flAlpha)) + vCurrentOriginPosition.y - vInitialPosition.y;
	flNewRotation = 0.0f; //placeholder
	};
	parms.flProjScriptTime = 5.0f;
*/

bool CSpellKineticBarrageWeak::cast(InstanceId const iCaster, InstanceId const iTarget)
{
	if(iCaster == INVALID_INSTANCE)
	{
		return false;
	}

	CShip *pCaster = SG::get_engine()->instance_get_checked<CShip>(iCaster);

	if(pCaster == nullptr)
	{
		return false;
	}
	for(int i = 0; i < 12; ++i)
	{
		WorldEventCallback spawnProjectiles = [iCaster, i](IWorld *pWorld)
		{
			CShip *pCaster = SG::get_engine()->instance_get_checked<CShip>(iCaster);

			if(pCaster == nullptr)
			{
				return;
			}

			SG::get_audio_manager()->play_sound(2);

			for(int j = 0; j < 3; ++j)
			{
				InstanceId uiProjectileId;
				CProjectile * pProjectile;

				CInstanceFactory::create<CProjectile>(uiProjectileId, pProjectile);

				WeaponArch const *pWeapArch = SG::get_game_data_manager()->get_arch<WeaponArch>(29);

				CProjectile::CreationParameters parms;
				float flParentRotation = pCaster->get_rotation();
				std::uniform_real_distribution<float> dist_rot(-8.0f, 8.0f);
				float flAngleOffset = dist_rot(SG::get_random());
				float flAngle = flParentRotation + flAngleOffset;
				Vector2f vInitialPosition = pCaster->get_position();
				Matrix2f mRotationMatrix = Matrix2f::rotation_matrix(flAngle);
				Vector2f vInheritedVelocity = pCaster->get_velocity();

				parms.vPosition = vInitialPosition;
				parms.flRotation = flAngle;
				parms.uiAppearanceId = pWeapArch->uiProjectileAppearance;
				parms.pWeapArch = pWeapArch;
				parms.bActivated = true;
				parms.iParentInstanceId = iCaster;
				parms.vVelocity = vInheritedVelocity + mRotationMatrix * Vector2f(2250.0f, 0.0f);
				parms.flMass = 0.1f;
				parms.flRadius = 5.0f;

				pProjectile->initialize(parms);

				SG::get_world()->instance_enqueue(pProjectile);
			}
		};

		SG::get_world()->enqueue_event_callback(i * 0.04f, spawnProjectiles);
	}

	return true;
}