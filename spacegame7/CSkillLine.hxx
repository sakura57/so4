#pragma once

#include <string>
#include "CSkill.hxx"

typedef unsigned int SkillLineId;

class CSkillLine
{
public:
	CSkillLine(std::string const &szName, std::string const &szDescription, std::vector<SkillId> const &vSkills);
	~CSkillLine();

	void get_name(std::string &);
	void get_description(std::string &);
	void get_skills(std::vector<SkillId> &);

private:
	std::string m_szName;
	std::string m_szDescription;
	std::vector<SkillId> m_vSkills;
};