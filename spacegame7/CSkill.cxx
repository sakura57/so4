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
#include "CSkill.hxx"

CSkill::CSkill(SkillId const uiSkillId, std::string const &szName, CStatBlock const &statBlock, std::vector<SpellId> const &vSpells, StaticTextureId uiIconId)
	: m_uiSkillId(uiSkillId), m_szName(szName), m_statBlock(statBlock), m_vSpellsGranted(vSpells), m_uiIcon(uiIconId)
{
	
}

CSkill::~CSkill()
{

}

void CSkill::get_name(std::string &szName)
{
	szName = this->m_szName;
}

void CSkill::get_spells(std::vector<SpellId> &vSpells)
{
	vSpells = this->m_vSpellsGranted;
}

void CSkill::get_stat_block(CStatBlock &statBlock)
{
	statBlock = this->m_statBlock;
}

SkillId CSkill::get_id(void)
{
	return this->m_uiSkillId;
}

StaticTextureId CSkill::get_icon_id(void)
{
	return this->m_uiIcon;
}