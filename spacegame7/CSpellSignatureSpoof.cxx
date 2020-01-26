#include "CSpellSignatureSpoof.hxx"
#include "SGLib.hxx"
#include "CShip.hxx"
#include "CStatusEngineOffline.hxx"

bool CSpellSignatureSpoof::cast(InstanceId const iCaster, InstanceId const iTarget)
{
	if(iCaster == INVALID_INSTANCE)
	{
		return false;
	}

	IWorldObject *pWorldObject = SG::get_engine()->instance_get_checked<IWorldObject>(iCaster);

	if(pWorldObject == nullptr)
	{
		return false;
	}

	SG::get_audio_manager()->play_sound(19);

	IWorld *pGameWorld = SG::get_world();

	int instances = 0;

	/*
	* we will walk instances. if they implement RadarVisible,
	* show them in the object list
	*/
	IWorldInstance *pInstanceWalk = pGameWorld->instance_walk_begin();

	if(pInstanceWalk == nullptr)
	{
		goto NO_INSTANCES_TO_RENDER;
	}

	do
	{
		if(pInstanceWalk == pWorldObject) continue;

		InstanceFlags uiFlags = pInstanceWalk->instance_get_flags();

		//does this instance implement IRadarVisible?
		if(uiFlags & CProjectile::InstanceFlag)
		{
			CProjectile * pProjectile = static_cast<CProjectile*>(pInstanceWalk);

			if(pProjectile->is_missile() && pProjectile->get_parent_instance() != iCaster)
			{
				float fDistance = pWorldObject->get_position().distance(pProjectile->get_position());

				if(fDistance < 750.0f)
				{
					std::uniform_real_distribution<float> dist_roll(0.0f, 1.0f);
					float flRoll = dist_roll(SG::get_random());

					if(flRoll < 0.5f)
					{
						InstanceId iNewTarget = pProjectile->get_parent_instance();
						pProjectile->set_parent_instance(iCaster);
						pProjectile->set_seek_target(iNewTarget);
					}
					else
					{
						pProjectile->set_lifetime_remaining(-1.0f);
					}
				}
			}
		}

	} while(pInstanceWalk = pGameWorld->instance_walk_next());
NO_INSTANCES_TO_RENDER:

	return true;
}