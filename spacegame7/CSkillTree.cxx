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