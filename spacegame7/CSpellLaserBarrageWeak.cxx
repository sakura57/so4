#include "CSpellLaserBarrageWeak.hxx"
#include "SGLib.hxx"
#include "CShip.hxx"
#include "CDestinedProjectile.hxx"

#define BARRAGE_RADIUS 100.0f
#define BARRAGE_LENGTH 350.0f

bool CSpellLaserBarrageWeak::cast(InstanceId const iCaster, InstanceId const iTarget)
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

			SG::get_audio_manager()->play_sound(1);

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

				parms.fDestinyFunction = [i, flAngle, mRotationMatrix](float const flAlpha, Vector2f &vNewPosition, float &flNewRotation)
				{
					float flMultiplier = -1.0f + (2.0f / 12) * i;
					float flSpeed = 1400.0f;

					vNewPosition.x = flSpeed * flAlpha;
					vNewPosition.y = flMultiplier * (-BARRAGE_RADIUS / (BARRAGE_LENGTH * BARRAGE_LENGTH) * (flSpeed * flAlpha) * (flSpeed * flAlpha) + 2.0f * BARRAGE_RADIUS / BARRAGE_LENGTH * (flSpeed * flAlpha));
					vNewPosition = mRotationMatrix * vNewPosition;

					Vector2f vDerivative = mRotationMatrix * Vector2f(flSpeed, flMultiplier * (flSpeed * flSpeed * -2.0f * BARRAGE_RADIUS / (BARRAGE_LENGTH * BARRAGE_LENGTH) * flAlpha + flSpeed * 2.0f * BARRAGE_RADIUS / BARRAGE_LENGTH));
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

				parms.fDestinyFunction = [i, flAngle, mRotationMatrix](float const flAlpha, Vector2f &vNewPosition, float &flNewRotation)
				{
					float flMultiplier = 1.0f - (2.0f / 12) * i;
					float flSpeed = 1400.0f;

					vNewPosition.x = flSpeed * flAlpha;
					vNewPosition.y = flMultiplier * (-BARRAGE_RADIUS / (BARRAGE_LENGTH * BARRAGE_LENGTH) * (flSpeed * flAlpha) * (flSpeed * flAlpha) + 2.0f * BARRAGE_RADIUS / BARRAGE_LENGTH * (flSpeed * flAlpha));
					vNewPosition = mRotationMatrix * vNewPosition;

					Vector2f vDerivative = mRotationMatrix * Vector2f(flSpeed, flMultiplier * (flSpeed * flSpeed * -2.0f * BARRAGE_RADIUS / (BARRAGE_LENGTH * BARRAGE_LENGTH) * flAlpha + flSpeed * 2.0f * BARRAGE_RADIUS / BARRAGE_LENGTH));
					flNewRotation = rad2deg(atan2(vDerivative.y, vDerivative.x)); //placeholder
				};
				parms.flProjScriptTime = 2.5f;

				pProjectile->initialize(parms);

				SG::get_world()->instance_enqueue(pProjectile);
			}
		};

		SG::get_world()->enqueue_event_callback(0.1f * i, spawnProjectiles);
	}

	return true;
}