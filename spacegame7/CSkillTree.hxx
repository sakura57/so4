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