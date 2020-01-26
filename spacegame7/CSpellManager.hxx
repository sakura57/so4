#pragma once

#include "Util.hxx"
#include "ISpellManager.hxx"
#include <vector>

class CSpellManager : public ISpellManager
{
public:
	CSpellManager();
	~CSpellManager();

	virtual void add_spell(SpellId const, ISpell *);
	virtual ISpell *get_spell(SpellId const);
private:
	Spinlock m_mFieldAccess;

	std::vector<ISpell *> m_vSpells;
};