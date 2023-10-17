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

#include "InterfacePanel.hxx"
#include "SGLib.hxx"
#include "CInSpaceState.hxx"
#include "CSectorTransitionState.hxx"
#include "CharacterCreationPanel.hxx"
#include "SettingsPanel.hxx"
#include "ArenaStartPanel.hxx"
#include "LoadGamePanel.hxx"

class MainMenuPanel : public InterfacePanel
{
public:
	MainMenuPanel()
		: m_bPanelActive(true)
	{};
	virtual ~MainMenuPanel()
	{};

	virtual void render_panel(float const);

	virtual bool panel_active(void)
	{
		return m_bPanelActive;
	};

	virtual bool prevents_game_input(void)
	{
		return false;
	};
private:
	bool m_bPanelActive;
};