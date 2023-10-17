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
#include "CSkillDataManager.hxx"

CSkillDataManager::CSkillDataManager()
{
	
}

CSkillDataManager::~CSkillDataManager()
{
	for (auto pSkillTree : this->m_vSkillTrees)
	{
		if (pSkillTree != nullptr)
		{
			delete pSkillTree;
		}
	}

	for (auto pSkillLine : this->m_vSkillLines)
	{
		if (pSkillLine != nullptr)
		{
			delete pSkillLine;
		}
	}

	for (auto pSkill : this->m_vSkills)
	{
		if (pSkill != nullptr)
		{
			delete pSkill;
		}
	}
}

void CSkillDataManager::add_skill(SkillId const uiId, CSkill *pSkill)
{
	if (this->m_vSkills.size() <= uiId)
	{
		this->m_vSkills.resize(uiId + 1);
	}

	this->m_vSkills[uiId] = pSkill;
}

CSkill *CSkillDataManager::get_skill(SkillId const uiId)
{
	return this->m_vSkills[uiId];
}

void CSkillDataManager::add_skill_line(SkillLineId const uiId, CSkillLine* pSkillLine)
{
	if (this->m_vSkillLines.size() <= uiId)
	{
		this->m_vSkillLines.resize(uiId + 1);
	}

	this->m_vSkillLines[uiId] = pSkillLine;
}

CSkillLine* CSkillDataManager::get_skill_line(SkillLineId const uiId)
{
	return this->m_vSkillLines[uiId];
}

void CSkillDataManager::add_skill_tree(SkillTreeId const uiId, CSkillTree* pSkillTree)
{
	if (this->m_vSkillTrees.size() <= uiId)
	{
		this->m_vSkillTrees.resize(uiId + 1);
	}

	this->m_vSkillTrees[uiId] = pSkillTree;
}

CSkillTree* CSkillDataManager::get_skill_tree(SkillTreeId const uiId)
{
	return this->m_vSkillTrees[uiId];
}

int CSkillDataManager::get_skills(void)
{
	return this->m_vSkills.size();
}

int CSkillDataManager::get_skill_lines(void)
{
	return this->m_vSkillLines.size();
}

int CSkillDataManager::get_skill_trees(void)
{
	return this->m_vSkillTrees.size();
}