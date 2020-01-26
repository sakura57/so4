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