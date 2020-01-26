#include "CSpellPerfectDefense.hxx"
#include "SGLib.hxx"
#include "CShip.hxx"
#include "CStatusPerfectDefense.hxx"

bool CSpellPerfectDefense::cast(InstanceId const iCaster, InstanceId const iTarget)
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

	SG::get_audio_manager()->play_sound(21);

	pCaster->inflict_status(new CStatusPerfectDefense(5.0f));

	return true;
}