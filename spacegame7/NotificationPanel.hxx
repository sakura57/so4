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
#include "CMainMenuState.hxx"

class NotificationPanel : public InterfacePanel
{
public:
	NotificationPanel(std::string const &szText)
		: m_bActive(true), m_szText(szText), m_flDuration(0.0f)
	{};
	virtual ~NotificationPanel()
	{};

	virtual void render_panel(float const);

	virtual bool panel_active(void)
	{
		return this->m_bActive;
	};

	virtual bool prevents_game_input(void)
	{
		return false;
	};

private:
	std::string m_szText;
	float m_flDuration;
	bool m_bActive;
};