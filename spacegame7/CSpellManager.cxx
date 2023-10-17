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