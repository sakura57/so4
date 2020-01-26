#include "CSpellLaserBarrageStrong.hxx"
#include "SGLib.hxx"
#include "CShip.hxx"
#include "CDestinedProjectile.hxx"

#define BARRAGE_RADIUS 80.0f
#define BARRAGE_LENGTH 335.0f

bool CSpellLaserBarrageStrong::cast(InstanceId const iCaster, InstanceId const iTarget)
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
	for(int i = 0; i < 14; ++i)
	{
		WorldEventCallback spawnProjectiles = [iCaster, i](IWorld *pWorld)
		{
			CShip *pCaster = SG::get_engine()->instance_get_checked<CShip>(iCaster);

			if(pCaster == nullptr)
			{
				return;
			}

			SG::get_audio_manager()->play_sound(5);

			//Create projectiles
			{
				InstanceId uiProjectileId;
				CDestinedProjectile * pProjectile;

				CInstanceFactory::create<CDestinedProjectile>(uiProjectileId, pProjectile);

				WeaponArch const *pWeapArch = SG::get_game_data_manager()->get_arch<WeaponArch>(31);

				CDestinedProjectile::CreationParameters parms;
				float flParentRotation = pCaster->get_rotation();
				//float flAngleOffset = 60.0f / 12 * i;
				float flAngle = flParentRotation;
				Vector2f vInitialPosition = pCaster->get_position();
				Matrix2f mRotationMatrix = Matrix2f::rotation_matrix(flAngle);
				Vector2f vInheritedVelocity = pCaster->get_velocity();

				parms.vPosition = vInitialPosition;
				parms.flRotation = 0.0f;
				parms.uiAppearanceId = pWeapArch->uiProjectileAppearance;
				parms.pWeapArch = pWeapArch;
				parms.bActivated = true;
				parms.iParentInstanceId = iCaster;
				parms.vVelocity = vInheritedVelocity + mRotationMatrix * Vector2f(2250.0f, 0.0f);
				parms.flMass = 0.1f;
				parms.flRadius = 5.0f;
				parms.flDamageMultiplier = 1.0f;

				parms.bHasLifetimeAfterDestiny = false;

				std::uniform_real_distribution<float> dist_rot(0.1f, 1.0f);
				float flMultiplier = dist_rot(SG::get_random());

				parms.fDestinyFunction = [i, flAngle, flMultiplier, mRotationMatrix](float const flAlpha, Vector2f &vNewPosition, float &flNewRotation)
				{
					float flSpeed = 1400.0f;

					vNewPosition.x = flSpeed * flAlpha;
					vNewPosition.y = -BARRAGE_RADIUS * flMultiplier * sin(vNewPosition.x / 100.0f);
					vNewPosition = mRotationMatrix * vNewPosition;

					Vector2f vDerivative = mRotationMatrix * Vector2f(flSpeed, -BARRAGE_RADIUS * flMultiplier * flSpeed * cos(flSpeed * flAlpha / 100.0f) / 100.0f);
					flNewRotation = rad2deg(atan2(vDerivative.y, vDerivative.x)); //placeholder
				};
				parms.flProjScriptTime = 2.5f;

				pProjectile->initialize(parms);

				SG::get_world()->instance_enqueue(pProjectile);
			}

			{
				InstanceId uiProjectileId;
				CDestinedProjectile * pProjectile;

				CInstanceFactory::create<CDestinedProjectile>(uiProjectileId, pProjectile);

				WeaponArch const *pWeapArch = SG::get_game_data_manager()->get_arch<WeaponArch>(31);

				CDestinedProjectile::CreationParameters parms;
				float flParentRotation = pCaster->get_rotation();
				//float flAngleOffset = 60.0f / 12 * i;
				float flAngle = flParentRotation;
				Vector2f vInitialPosition = pCaster->get_position();
				Matrix2f mRotationMatrix = Matrix2f::rotation_matrix(flAngle);
				Vector2f vInheritedVelocity = pCaster->get_velocity();

				parms.vPosition = vInitialPosition;
				parms.flRotation = 0.0f;
				parms.uiAppearanceId = pWeapArch->uiProjectileAppearance;
				parms.pWeapArch = pWeapArch;
				parms.bActivated = true;
				parms.iParentInstanceId = iCaster;
				parms.vVelocity = vInheritedVelocity + mRotationMatrix * Vector2f(2250.0f, 0.0f);
				parms.flMass = 0.1f;
				parms.flRadius = 5.0f;
				parms.flDamageMultiplier = 1.0f;

				parms.bHasLifetimeAfterDestiny = false;

				std::uniform_real_distribution<float> dist_rot(0.1f, 1.0f);
				float flMultiplier = dist_rot(SG::get_random());

				parms.fDestinyFunction = [i, flAngle, flMultiplier, mRotationMatrix](float const flAlpha, Vector2f &vNewPosition, float &flNewRotation)
				{
					float flSpeed = 1400.0f;

					vNewPosition.x = flSpeed * flAlpha;
					vNewPosition.y = flMultiplier * BARRAGE_RADIUS * sin(vNewPosition.x / 100.0f);
					vNewPosition = mRotationMatrix * vNewPosition;

					Vector2f vDerivative = mRotationMatrix * Vector2f(flSpeed, flMultiplier * BARRAGE_RADIUS * flSpeed * cos(flSpeed * flAlpha / 100.0f) / 100.0f);
					flNewRotation = rad2deg(atan2(vDerivative.y, vDerivative.x)); //placeholder
				};
				parms.flProjScriptTime = 2.5f;

				pProjectile->initialize(parms);

				SG::get_world()->instance_enqueue(pProjectile);
			}
		};

		SG::get_world()->enqueue_event_callback(0.1f * i, spawnProjectiles);
	}

	for(int i = 0; i < 4; ++i)
	{
		WorldEventCallback spawnProjectiles = [iCaster, i](IWorld *pWorld)
		{
			CShip *pCaster = SG::get_engine()->instance_get_checked<CShip>(iCaster);

			if(pCaster == nullptr)
			{
				return;
			}

			SG::get_audio_manager()->play_sound(6);

			for(int j = 0; j < 3; ++j)
			{
				InstanceId uiProjectileId;
				CProjectile * pProjectile;

				CInstanceFactory::create<CProjectile>(uiProjectileId, pProjectile);

				WeaponArch const *pWeapArch = SG::get_game_data_manager()->get_arch<WeaponArch>(31);

				CProjectile::CreationParameters parms;
				float flParentRotation = pCaster->get_rotation();
				float flAngleOffset = j * 10.0f;
				float flAngle = flParentRotation - 10.0f + flAngleOffset;
				Vector2f vInitialPosition = pCaster->get_position();
				Matrix2f mRotationMatrix = Matrix2f::rotation_matrix(flAngle);
				Vector2f vInheritedVelocity = pCaster->get_velocity();

				parms.vPosition = vInitialPosition;
				parms.flRotation = flAngle;
				parms.uiAppearanceId = pWeapArch->uiProjectileAppearance;
				parms.pWeapArch = pWeapArch;
				parms.bActivated = true;
				parms.iParentInstanceId = iCaster;
				parms.vVelocity = vInheritedVelocity + mRotationMatrix * Vector2f(2500.0f, 0.0f);
				parms.flMass = 0.1f;
				parms.flRadius = 5.0f;

				pProjectile->initialize(parms);

				SG::get_world()->instance_enqueue(pProjectile);
			}
		};

		SG::get_world()->enqueue_event_callback(0.35f * i, spawnProjectiles);
	}

	return true;
}