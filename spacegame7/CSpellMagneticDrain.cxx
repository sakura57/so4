#include "CSpellMagneticDrain.hxx"
#include "SGLib.hxx"
#include "CShip.hxx"

bool CSpellMagneticDrain::cast(InstanceId const iCaster, InstanceId const iTarget)
{
	if(iCaster == INVALID_INSTANCE || iTarget == INVALID_INSTANCE)
	{
		return false;
	}

	CShip *pCaster = SG::get_engine()->instance_get_checked<CShip>(iCaster);

	if(pCaster == nullptr)
	{
		return false;
	}

	CEquippedObject *pTarget = SG::get_engine()->instance_get_checked<CEquippedObject>(iTarget);

	if(pTarget == nullptr)
	{
		return false;
	}

	SG::get_audio_manager()->play_sound(17);
	SG::get_particle_manager()->add_particle(13, pTarget->get_position(), pTarget->get_velocity(), 0.0f, 0.0f);
	SG::get_particle_manager()->add_particle(14, pCaster->get_position(), pCaster->get_velocity(), 0.0f, 0.0f);
	
	//We want to drain UP TO 30% of OUR max energy, from the target's capacitor
	float flEnergyToDrain = clamp(pTarget->get_capacitor_energy(), 0.0f, 0.30f * pCaster->get_max_capacitor_energy());

	//Restoring will only restore from the energy we've drained, the sum can't be more than our max energy
	float flPostRestoreEnergy = clamp(pCaster->get_capacitor_energy() + flEnergyToDrain, 0.0f, pCaster->get_max_capacitor_energy());

	pCaster->set_capacitor_energy(flPostRestoreEnergy);
	pTarget->perform_hostile_action(iCaster);

	return true;
}