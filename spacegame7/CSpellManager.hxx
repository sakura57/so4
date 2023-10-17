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