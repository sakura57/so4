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
#include "CSpellMissileSwarmStrong.hxx"
#include "SGLib.hxx"
#include "CShip.hxx"
#include "CDestinedProjectile.hxx"

bool CSpellMissileSwarmStrong::cast(InstanceId const iCaster, InstanceId const iTarget)
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

	for(int i = 0; i < 2; ++i)
	{
		WorldEventCallback spawnProjectiles = [iCaster, i](IWorld *pWorld)
		{
			CShip *pCaster = SG::get_engine()->instance_get_checked<CShip>(iCaster);

			if(pCaster == nullptr)
			{
				return;
			}

			SG::get_audio_manager()->play_sound(9);

			//Create projectiles
			for(int j = 0; j < 2; ++j)
			{
				InstanceId uiProjectileId;
				CProjectile * pProjectile;

				CInstanceFactory::create<CProjectile>(uiProjectileId, pProjectile);

				MissileArch const *pWeapArch = SG::get_game_data_manager()->get_arch<MissileArch>(34);

				std::uniform_real_distribution<float> dist_rot(-20.0f, 20.0f);
				float flAngleOffset = dist_rot(SG::get_random());

				CProjectile::CreationParameters parms;
				float flParentRotation = pCaster->get_rotation();
				float flAngle = flParentRotation + flAngleOffset;
				Vector2f vInitialPosition = pCaster->get_position();
				Matrix2f mRotationMatrix = Matrix2f::rotation_matrix(flAngle);
				Vector2f vInheritedVelocity = pCaster->get_velocity();

				parms.vPosition = vInitialPosition;
				parms.flRotation = 0.0f;
				parms.uiAppearanceId = pWeapArch->uiProjectileAppearance;
				parms.pWeapArch = pWeapArch;
				parms.bActivated = true;
				parms.iParentInstanceId = iCaster;
				parms.vVelocity = vInheritedVelocity + mRotationMatrix * Vector2f(500.0f, 0.0f);
				parms.flMass = 0.1f;
				parms.flRadius = 5.0f;
				parms.flDamageMultiplier = 1.0f;

				pProjectile->initialize(parms);

				SG::get_world()->instance_enqueue(pProjectile);
			}
		};

		SG::get_world()->enqueue_event_callback(0.4f * i, spawnProjectiles);
	}

	for(int i = 0; i < 6; ++i)
	{
		WorldEventCallback spawnProjectiles = [iCaster, i](IWorld *pWorld)
		{
			CShip *pCaster = SG::get_engine()->instance_get_checked<CShip>(iCaster);

			if(pCaster == nullptr)
			{
				return;
			}

			SG::get_audio_manager()->play_sound(9);

			//Create projectiles
			{
				InstanceId uiProjectileId;
				CProjectile * pProjectile;

				CInstanceFactory::create<CProjectile>(uiProjectileId, pProjectile);

				MissileArch const *pWeapArch = SG::get_game_data_manager()->get_arch<MissileArch>(34);

				std::uniform_real_distribution<float> dist_rot(-20.0f, 20.0f);
				float flAngleOffset = dist_rot(SG::get_random());

				CProjectile::CreationParameters parms;
				float flParentRotation = pCaster->get_rotation();
				float flAngle = flParentRotation + flAngleOffset;
				Vector2f vInitialPosition = pCaster->get_position();
				Matrix2f mRotationMatrix = Matrix2f::rotation_matrix(flAngle);
				Vector2f vInheritedVelocity = pCaster->get_velocity();

				parms.vPosition = vInitialPosition;
				parms.flRotation = 0.0f;
				parms.uiAppearanceId = pWeapArch->uiProjectileAppearance;
				parms.pWeapArch = pWeapArch;
				parms.bActivated = true;
				parms.iParentInstanceId = iCaster;
				parms.vVelocity = vInheritedVelocity + mRotationMatrix * Vector2f(500.0f, 0.0f);
				parms.flMass = 0.1f;
				parms.flRadius = 5.0f;
				parms.flDamageMultiplier = 1.0f;

				pProjectile->initialize(parms);

				SG::get_world()->instance_enqueue(pProjectile);
			}
		};

		SG::get_world()->enqueue_event_callback(0.8f + 0.08f * i, spawnProjectiles);
	}

	return true;
}