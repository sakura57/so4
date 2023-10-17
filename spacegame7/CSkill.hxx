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
#pragma once

#include <string>
#include <vector>
#include "CStatBlock.hxx"
#include "ISpell.hxx"

typedef unsigned int SkillId;

class CSkill
{
public:
	CSkill(SkillId const, std::string const &, CStatBlock const &, std::vector<SpellId> const &, StaticTextureId uiIcon);
	~CSkill();

	void get_spells(std::vector<SpellId> &);
	void get_name(std::string &);
	void get_stat_block(CStatBlock &);

	SkillId get_id(void);

	StaticTextureId get_icon_id(void);

private:
	std::string m_szName;

	StaticTextureId m_uiIcon;
	CStatBlock m_statBlock;
	std::vector<SpellId> m_vSpellsGranted;
	SkillId m_uiSkillId;
};