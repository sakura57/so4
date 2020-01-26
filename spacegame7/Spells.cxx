#include "Spells.hxx"
#include "CSpellNavComVirus.hxx"
#include "CSpellNanobotStream.hxx"
#include "CSpellNanobotSwarm.hxx"
#include "CSpellMagneticDrain.hxx"
#include "CSpellSignatureSpoof.hxx"
#include "CSpellPerfectDefense.hxx"
#include "CSpellKineticBarrageWeak.hxx"
#include "CSpellKineticBarrageStrong.hxx"
#include "CSpellLaserBarrageWeak.hxx"
#include "CSpellLaserBarrageStrong.hxx"
#include "CSpellMissileSwarmWeak.hxx"
#include "CSpellMissileSwarmStrong.hxx"

void add_all_spells(ISpellManager *pSpellManager)
{
	//add all spells to the given spell manager
	//due to the specific nature of spells,
	//they will remain hardcoded like this until further notice

	pSpellManager->add_spell(SPELL_NAVCOM_VIRUS, new CSpellNavComVirus);
	pSpellManager->add_spell(SPELL_NANOBOT_STREAM, new CSpellNanobotStream);
	pSpellManager->add_spell(SPELL_NANOBOT_SWARM, new CSpellNanobotSwarm);
	pSpellManager->add_spell(SPELL_MAGNETIC_DRAIN, new CSpellMagneticDrain);
	pSpellManager->add_spell(SPELL_SIGNATURE_SPOOF, new CSpellSignatureSpoof);
	pSpellManager->add_spell(SPELL_PERFECT_DEFENSE, new CSpellPerfectDefense);
	pSpellManager->add_spell(SPELL_KINETIC_BARRAGE_WEAK, new CSpellKineticBarrageWeak);
	pSpellManager->add_spell(SPELL_KINETIC_BARRAGE_STRONG, new CSpellKineticBarrageStrong);
	pSpellManager->add_spell(SPELL_LASER_BARRAGE_WEAK, new CSpellLaserBarrageWeak);
	pSpellManager->add_spell(SPELL_LASER_BARRAGE_STRONG, new CSpellLaserBarrageStrong);
	pSpellManager->add_spell(SPELL_MISSILE_SWARM_WEAK, new CSpellMissileSwarmWeak);
	pSpellManager->add_spell(SPELL_MISSILE_SWARM_STRONG, new CSpellMissileSwarmStrong);
}