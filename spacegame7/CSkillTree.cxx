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
#include "CSkillTree.hxx"

CSkillTree::CSkillTree(std::string const &szName, std::string const &szDescription, std::vector<SkillLineId> const &skillLines, unsigned int const uiFinalSkill)
	: m_szName(szName), m_szDescription(szDescription), m_vSkillLines(skillLines), m_uiFinalSkill(uiFinalSkill)
{
	
}

CSkillTree::~CSkillTree()
{
	
}

void CSkillTree::get_name(std::string &szName)
{
	szName = this->m_szName;
}

void CSkillTree::get_description(std::string &szDescription)
{
	szDescription = this->m_szDescription;
}

void CSkillTree::get_skill_lines(std::vector<SkillLineId> &vSkillLines)
{
	vSkillLines = this->m_vSkillLines;
}

void CSkillTree::get_final_skill(unsigned int& uiFinalSkill)
{
	uiFinalSkill = this->m_uiFinalSkill;
}