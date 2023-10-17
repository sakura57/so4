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
#include "InGameMenuPanel.hxx"
#include "SGLib.hxx"
#include "CMainMenuState.hxx"
#include "CGameExitState.hxx"

int InGameMenuPanel::m_iPanelInstances = 0;

InGameMenuPanel::InGameMenuPanel()
{
	if(++this->m_iPanelInstances > 1)
	{
		this->m_bPanelActive = false;
	}
	else
	{
		this->m_bPanelActive = true;
	}
}

InGameMenuPanel::~InGameMenuPanel()
{
	--this->m_iPanelInstances;
}

void InGameMenuPanel::render_panel(float const flDelta)
{
	ImGui::SetNextWindowPosCenter(ImGuiCond_Once | ImGuiCond_Appearing);

	ImGui::Begin(
		"In-Game Menu",
		nullptr,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse
	);

	if(ImGui::Button("Exit to Menu"))
	{
		SG::get_game_state_manager()->transition_game_state(new CMainMenuState);
	}

	if(ImGui::Button("Exit to Desktop"))
	{
		SG::get_game_state_manager()->transition_game_state(new CGameExitState);
	}

	ImGui::End();
}