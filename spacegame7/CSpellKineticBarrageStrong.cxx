#include "CSpellKineticBarrageStrong.hxx"
#include "SGLib.hxx"
#include "CShip.hxx"
#include "CDestinedProjectile.hxx"

bool CSpellKineticBarrageStrong::cast(InstanceId const iCaster, InstanceId const iTarget)
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
	for(int i = 0; i < 16; ++i)
	{
		WorldEventCallback spawnProjectiles = [iCaster, i](IWorld *pWorld)
		{
			CShip *pCaster = SG::get_engine()->instance_get_checked<CShip>(iCaster);

			if(pCaster == nullptr)
			{
				return;
			}

			SG::get_audio_manager()->play_sound(3);

			for(int j = 0; j < 2; ++j)
			{
				InstanceId uiProjectileId;
				CProjectile * pProjectile;

				CInstanceFactory::create<CProjectile>(uiProjectileId, pProjectile);

				WeaponArch const *pWeapArch = SG::get_game_data_manager()->get_arch<WeaponArch>(30);

				CProjectile::CreationParameters parms;
				float flParentRotation = pCaster->get_rotation();
				std::uniform_real_distribution<float> dist_rot(-12.0f, 12.0f);
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
				parms.vVelocity = vInheritedVelocity + mRotationMatrix * Vector2f(1750.0f, 0.0f);
				parms.flMass = 0.1f;
				parms.flRadius = 5.0f;

				pProjectile->initialize(parms);

				SG::get_world()->instance_enqueue(pProjectile);
			}
		};

		SG::get_world()->enqueue_event_callback(i * 0.065f, spawnProjectiles);
	}

	return true;
}