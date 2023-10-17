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
#include "CBaseCharacterEntity.hxx"
#include "SGLib.hxx"

CBaseCharacterEntity::CBaseCharacterEntity(Loadout const *pLoadout, std::string const &szName, bool const bPlayer)
	: m_bPlayer(bPlayer), m_szName(szName)
{
	this->m_lLoadout = *pLoadout;

	this->m_iLevel = 1;
	this->m_ulCurrentExp = 0UL;
	this->m_ulExpToNextLevel = 100UL;

	this->m_flMana = 100.0f;
	this->m_flMaxMana = 100.0f;
	this->m_flManaRegen = this->m_flMaxMana * 0.01f;

	this->m_uiFreeSkillPoints = 0U;

	this->m_pStatBlock = new CStatBlock;
}

CBaseCharacterEntity::~CBaseCharacterEntity()
{
	delete this->m_pStatBlock;
}

std::string CBaseCharacterEntity::get_name(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_szName;
}

void CBaseCharacterEntity::set_name(std::string const &szName)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_szName = szName;
}

IEntityInventory *CBaseCharacterEntity::get_inventory(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return nullptr;
}

IMaterialBank* CBaseCharacterEntity::get_material_bank(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return nullptr;
}

Loadout CBaseCharacterEntity::get_loadout(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_lLoadout;
}

void CBaseCharacterEntity::demount_item(int const iIndex)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(iIndex < 0 || iIndex >= this->m_lLoadout.size())
	{
		throw SGException("Attempted to demount from invalid loadout position");
	}

	this->m_lLoadout[iIndex] = 0;
}

void CBaseCharacterEntity::mount_item(int const iIndex, ArchId const iArch)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(iIndex < 0 || iIndex >= this->m_lLoadout.size())
	{
		throw SGException("Attempted to demount from invalid loadout position");
	}

	this->m_lLoadout[iIndex] = iArch;
}

void CBaseCharacterEntity::set_loadout(Loadout const &lLoadout)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_lLoadout = lLoadout;
}

SpellSet CBaseCharacterEntity::get_known_spells(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_vKnownSpells;
}

SpellSet CBaseCharacterEntity::get_mapped_spells(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_vMappedSpells;
}

void CBaseCharacterEntity::set_mapped_spells(SpellSet const &vMappedSpells)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vMappedSpells = vMappedSpells;
}

void CBaseCharacterEntity::learn_spell(SpellId const uiSpellId)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vKnownSpells.push_back(uiSpellId);
}

void CBaseCharacterEntity::reset_spells(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vKnownSpells.clear();
	this->m_vMappedSpells.clear();
}

void CBaseCharacterEntity::cast_spell(int const iSpellSlot, InstanceId const iPlayerInstance, InstanceId const iTargetInstance)
{
	if(iPlayerInstance == INVALID_INSTANCE)
	{
		return;
	}

	SCOPE_LOCK(this->m_mFieldAccess);

	if(iSpellSlot < 0 || iSpellSlot >= this->m_vMappedSpells.size())
	{
		return;
	}

	SpellId const iSpellId = this->m_vMappedSpells[iSpellSlot];

	if(iSpellId == 0)
	{
		return;
	}

	ISpell *pSpell = SG::get_spell_manager()->get_spell(iSpellId);

	if(pSpell == nullptr)
	{
		return;
	}

	float flManaCost = pSpell->get_mana_cost();

	if(pSpell->get_mana_cost() > this->m_flMana)
	{
		if(this->m_bPlayer)
		{
			SG::get_game_state_manager()->get_game_state()->state_send_notification("Not enough mind points");
		}
		return;
	}

	if(pSpell->cast(iPlayerInstance, iTargetInstance))
	{
		this->m_vSpellCooldown[iSpellId] = pSpell->get_cooldown();
		this->m_flMana -= flManaCost;
	}
}

bool CBaseCharacterEntity::can_cast_spell(int const iSpellSlot)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(iSpellSlot < 0 || iSpellSlot >= this->m_vMappedSpells.size())
	{
		return false;
	}

	SpellId const iSpellId = this->m_vMappedSpells[iSpellSlot];

	if(iSpellId == 0)
	{
		return false;
	}

	return this->m_vSpellCooldown[iSpellId] <= 0.0f;
}

float CBaseCharacterEntity::get_spell_cooldown_time_remaining(int const iSpellSlot)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(iSpellSlot < 0 || iSpellSlot >= this->m_vMappedSpells.size())
	{
		return -1.0f;
	}

	SpellId const iSpellId = this->m_vMappedSpells[iSpellSlot];

	if(iSpellId == 0)
	{
		return -1.0f;
	}

	return this->m_vSpellCooldown[iSpellId];
}

void CBaseCharacterEntity::tick(float const flDelta)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	//Handle mana regen
	if(this->m_flMana < this->m_flMaxMana)
	{
		float flNewMana = this->m_flMana + flDelta * this->m_flManaRegen;
		this->m_flMana = clamp(flNewMana, 0.0f, this->m_flMaxMana);
	}

	//Handle spell cooldowns
	for(auto &piCooldownEntry : this->m_vSpellCooldown)
	{
		if(piCooldownEntry.second >= 0.0f)
		{
			piCooldownEntry.second -= flDelta;
		}
	}
}

float CBaseCharacterEntity::get_mana(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_flMana;
}

void CBaseCharacterEntity::set_mana(float const flNewMana)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flMana = flNewMana;
}

float CBaseCharacterEntity::get_max_mana(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_flMaxMana;
}

int CBaseCharacterEntity::get_level(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_iLevel;
}

void CBaseCharacterEntity::set_level(int const iLevel)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_iLevel = iLevel;
	this->m_ulExpToNextLevel = CBaseCharacterEntity::get_exp_from_level(this->m_iLevel);
	this->m_ulCurrentExp = 0UL;

	this->update_mana();
}

void CBaseCharacterEntity::gain_exp(unsigned long int const ulExp)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	unsigned long int ulSumExp = this->m_ulCurrentExp + ulExp;

	if(ulSumExp >= this->m_ulExpToNextLevel)
	{
		++this->m_iLevel;
		++this->m_uiFreeSkillPoints;

		this->m_ulExpToNextLevel = CBaseCharacterEntity::get_exp_from_level(this->m_iLevel);
		this->m_ulCurrentExp = 0UL;

		this->update_mana();

		if(this->m_bPlayer)
		{
			SG::get_game_state_manager()->get_game_state()->state_send_notification("LEVEL UP");
		}
	}
	else
	{
		this->m_ulCurrentExp = ulSumExp;
	}
}

unsigned long CBaseCharacterEntity::get_exp_to_next_level(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_ulExpToNextLevel;
}

unsigned long CBaseCharacterEntity::get_current_exp(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_ulCurrentExp;
}

unsigned long int CBaseCharacterEntity::get_exp_from_level(int const iLevel)
{
	//TODO: find a more efficient algorithm to calculate this
	std::vector<unsigned long> levels(iLevel, 0UL);

	levels[0] = 100UL;

	int i = 0;

	for(i = 1; i < iLevel; ++i)
	{
		levels[i] = (unsigned long)(1.1 * levels[i - 1]);
	}

	unsigned long xp = 0UL;
	for(i = 0; i < iLevel; ++i)
	{
		xp += levels[i];
	}

	return xp;
}

float CBaseCharacterEntity::get_mana_at_level(int const iLevel)
{
	return (float)(100.0 + (double)(iLevel - 1)) * pow((double)iLevel * 1.1, 0.4);
}

void CBaseCharacterEntity::update_mana(void)
{
	this->m_flMaxMana = CBaseCharacterEntity::get_mana_at_level(this->m_iLevel);
	this->m_flManaRegen = 0.01f * this->m_flMaxMana;
}

CStatBlock *CBaseCharacterEntity::get_stat_block(void)
{
	return this->m_pStatBlock;
}

unsigned int CBaseCharacterEntity::get_ship(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_uiShipId;
}

void CBaseCharacterEntity::set_ship(unsigned int const uiShipId)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_uiShipId = uiShipId;
}

bool CBaseCharacterEntity::has_skill(SkillId const skillId)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	for (int i = 0; i < this->m_vSkills.size(); ++i)
	{
		if (this->m_vSkills[i] == skillId)
		{
			return true;
		}
	}

	return false;
}

bool CBaseCharacterEntity::give_skill(SkillId const skillId)
{
	if (this->has_skill(skillId))
	{
		//if we already have the skill, just ignore.
		//we do this before SCOPE_LOCK to prevent deadlock

		return false;
	}

	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vSkills.push_back(skillId);

	return true;
}

bool CBaseCharacterEntity::grant_skill(SkillId const skillId)
{
	if (this->has_skill(skillId))
	{
		//if we already have the skill, just ignore.
		//we do this before SCOPE_LOCK to prevent deadlock

		return false;
	}

	SCOPE_LOCK(this->m_mFieldAccess);

	if (this->m_uiFreeSkillPoints < 1)
	{
		//if we cannot acquire the skill due to lack of
		//skill points, ignore

		return false;
	}

	--this->m_uiFreeSkillPoints;

	this->m_vSkills.push_back(skillId);

	CSkill* pSkill = SG::get_game_data_manager()->get_skill_data_manager()->get_skill(skillId);

	/*
	 * Merge the stat block of the skill to our own,
	 * giving the character the attributes granted by
	 * the skill
	 */
	CStatBlock skillStatsGranted;
	pSkill->get_stat_block(skillStatsGranted);

	this->m_pStatBlock->merge(&skillStatsGranted);

	/*
	 * Grant all the abilities granted by the skill.
	 */
	std::vector<SpellId> vGrantedSpells;
	pSkill->get_spells(vGrantedSpells);

	for (SpellId const spell : vGrantedSpells)
	{
		this->m_vKnownSpells.push_back(spell);
	}

	return true;
}

unsigned int CBaseCharacterEntity::get_skill_points_available(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_uiFreeSkillPoints;
}

float CBaseCharacterEntity::get_stat(Stat const stat)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_pStatBlock->get_stat(stat);
}

void CBaseCharacterEntity::set_stats(CStatBlock const& statBlock)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	*this->m_pStatBlock = statBlock;
}

void CBaseCharacterEntity::set_free_skill_points(unsigned int const uiSkillPoints)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_uiFreeSkillPoints = uiSkillPoints;
}

void CBaseCharacterEntity::get_skills(std::vector<SkillId> &skills)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	skills = this->m_vSkills;
}

void CBaseCharacterEntity::clear_skills(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vSkills.clear();
}