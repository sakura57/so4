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

#include "CSkill.hxx"
#include "CSkillLine.hxx"
#include "CSkillTree.hxx"

class CSkillDataManager
{
public:
	CSkillDataManager();
	~CSkillDataManager();

	void add_skill(SkillId const uiId, CSkill* pSkill);
	void add_skill_line(SkillLineId const uiId, CSkillLine* pSkillLine);
	void add_skill_tree(SkillTreeId const uiId, CSkillTree* pSkillTree);

	CSkill* get_skill(SkillId const uiId);
	CSkillLine* get_skill_line(SkillLineId const uiId);
	CSkillTree* get_skill_tree(SkillTreeId const uiId);

	int get_skills(void);
	int get_skill_lines(void);
	int get_skill_trees(void);

private:
	std::vector<CSkill*> m_vSkills;
	std::vector<CSkillLine*> m_vSkillLines;
	std::vector<CSkillTree*> m_vSkillTrees;
};