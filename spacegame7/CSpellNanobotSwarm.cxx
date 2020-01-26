#include "CSpellNanobotSwarm.hxx"
#include "SGLib.hxx"
#include "CShip.hxx"
#include "CStatusNanobotSwarm.hxx"

bool CSpellNanobotSwarm::cast(InstanceId const iCaster, InstanceId const iTarget)
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

	SG::get_audio_manager()->play_sound(15);
	SG::get_particle_manager()->add_particle(11, pCaster->get_position(), pCaster->get_velocity(), 0.0f, 0.0f);

	pCaster->inflict_status(new CStatusNanobotSwarm(2.0f));

	return true;
}