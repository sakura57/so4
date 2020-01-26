#include "CSkill.hxx"

CSkill::CSkill(SkillId const uiSkillId, std::string const &szName, CStatBlock const &statBlock, std::vector<SpellId> const &vSpells, StaticTextureId uiIconId)
	: m_uiSkillId(uiSkillId), m_szName(szName), m_statBlock(statBlock), m_vSpellsGranted(vSpells), m_uiIcon(uiIconId)
{
	
}

CSkill::~CSkill()
{

}

void CSkill::get_name(std::string &szName)
{
	szName = this->m_szName;
}

void CSkill::get_spells(std::vector<SpellId> &vSpells)
{
	vSpells = this->m_vSpellsGranted;
}

void CSkill::get_stat_block(CStatBlock &statBlock)
{
	statBlock = this->m_statBlock;
}

SkillId CSkill::get_id(void)
{
	return this->m_uiSkillId;
}

StaticTextureId CSkill::get_icon_id(void)
{
	return this->m_uiIcon;
}