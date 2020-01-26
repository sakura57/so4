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