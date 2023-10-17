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

#include <sstream>

#include "InterfacePanel.hxx"
#include "ICharacterEntity.hxx"

struct AbilityDrop
{
	SpellId iSpellId;
	int iSpellSlot;
};

class AbilityToolbar : public InterfacePanel
{
public:
	AbilityToolbar(InstanceId iInstanceId, ICharacterEntity *pCharEntity)
		: m_pCharEntity(pCharEntity), m_iInstanceId(iInstanceId)
	{
	};
	virtual ~AbilityToolbar()
	{};

	virtual void render_panel(float const flDelta);

	static void do_ability_tooltip(ISpell *pSpell);

	virtual bool panel_active(void)
	{
		return true;
	};

	virtual bool prevents_game_input(void)
	{
		return false;
	};

private:
	ICharacterEntity *m_pCharEntity;
	InstanceId m_iInstanceId;
};