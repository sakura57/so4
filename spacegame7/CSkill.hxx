#pragma once

#include <string>
#include <vector>
#include "CStatBlock.hxx"
#include "ISpell.hxx"

typedef unsigned int SkillId;

class CSkill
{
public:
	CSkill(SkillId const, std::string const &, CStatBlock const &, std::vector<SpellId> const &, StaticTextureId uiIcon);
	~CSkill();

	void get_spells(std::vector<SpellId> &);
	void get_name(std::string &);
	void get_stat_block(CStatBlock &);

	SkillId get_id(void);

	StaticTextureId get_icon_id(void);

private:
	std::string m_szName;

	StaticTextureId m_uiIcon;
	CStatBlock m_statBlock;
	std::vector<SpellId> m_vSpellsGranted;
	SkillId m_uiSkillId;
};