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

#include <future>

#include "InterfacePanel.hxx"

class PlayerDiedPanel : public InterfacePanel
{
public:
	PlayerDiedPanel(std::string const &szDeathString)
		: m_szDeathString(szDeathString)
	{
		if(++PlayerDiedPanel::m_iPanelInstances > 1)
		{
			this->m_bPanelActive = false;
		}
		else
		{
			this->m_bPanelActive = true;
		}
	};
	virtual ~PlayerDiedPanel()
	{
		--PlayerDiedPanel::m_iPanelInstances;
	};

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void);

	virtual bool prevents_game_input(void)
	{
		return true;
	};

private:
	std::string m_szDeathString;
	bool m_bPanelActive;
	std::future<bool> m_bGameLoadAttempt;

	static int m_iPanelInstances;
};