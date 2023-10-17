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
#include "CSkillLine.hxx"

CSkillLine::CSkillLine(std::string const &szName, std::string const &szDescription, std::vector<SkillId> const &vSkills)
	: m_szName(szName), m_szDescription(szDescription), m_vSkills(vSkills)
{
}

CSkillLine::~CSkillLine()
{
	
}

void CSkillLine::get_name(std::string &szName)
{
	szName = this->m_szName;
}

void CSkillLine::get_description(std::string &szDescription)
{
	szDescription = this->m_szDescription;
}

void CSkillLine::get_skills(std::vector<SkillId> &vSkills)
{
	vSkills = this->m_vSkills;
}