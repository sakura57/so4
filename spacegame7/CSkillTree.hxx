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
#include "CSkillLine.hxx"

typedef unsigned int SkillTreeId;

class CSkillTree
{
public:
	CSkillTree(std::string const &, std::string const &, std::vector<SkillLineId> const &, unsigned int const);
	~CSkillTree();

	void get_name(std::string &);
	void get_description(std::string &);
	void get_skill_lines(std::vector<SkillLineId> &);
	void get_final_skill(unsigned int &);
private:
	std::string m_szName;
	std::string m_szDescription;
	std::vector<SkillLineId> m_vSkillLines;
	SkillId m_uiFinalSkill;
};