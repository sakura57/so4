#include "CSpellNavComVirus.hxx"
#include "SGLib.hxx"
#include "CShip.hxx"
#include "CStatusEngineOffline.hxx"

bool CSpellNavComVirus::cast(InstanceId const iCaster, InstanceId const iTarget)
{
	if(iCaster == INVALID_INSTANCE || iTarget == INVALID_INSTANCE)
	{
		return false;
	}

	CShip *pCaster = SG::get_engine()->instance_get_checked<CShip>(iCaster);
	CEquippedObject *pTarget = SG::get_engine()->instance_get_checked<CEquippedObject>(iTarget);

	if(pCaster == nullptr)
	{
		return false;
	}

	if(pTarget == nullptr)
	{
		return false;
	}

	SG::get_audio_manager()->play_sound(14);
	SG::get_particle_manager()->add_particle(12, pTarget->get_position(), pTarget->get_velocity(), 0.0f, 0.0f);

	pTarget->inflict_status(new CStatusEngineOffline(5.0f));
	pTarget->perform_hostile_action(iCaster);

	return true;
}