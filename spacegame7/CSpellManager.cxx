#include "CSpellManager.hxx"

CSpellManager::CSpellManager()
{
	
}

CSpellManager::~CSpellManager()
{
	
}

void CSpellManager::add_spell(SpellId const uiSpellId, ISpell *pSpell)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(uiSpellId >= this->m_vSpells.size())
	{
		this->m_vSpells.resize(uiSpellId + 1);
	}

	this->m_vSpells[uiSpellId] = pSpell;
}

ISpell *CSpellManager::get_spell(SpellId const uiSpellId)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(uiSpellId >= this->m_vSpells.size())
	{
		return nullptr;
	}

	return this->m_vSpells[uiSpellId];
}