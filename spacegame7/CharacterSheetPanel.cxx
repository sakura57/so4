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
#include "CharacterSheetPanel.hxx"

int CharacterSheetPanel::m_iPanelInstances = 0;

void CharacterSheetPanel::render_panel(float const flDelta)
{
	if(this->m_pEntity == nullptr)
	{
		return;
	}

	ImGui::Begin("Character");

	std::string szName = this->m_pEntity->get_name();
	int iCurrentLevel = this->m_pEntity->get_level();
	unsigned long ulCurrentExp = this->m_pEntity->get_current_exp();
	unsigned long ulExpRequired = this->m_pEntity->get_exp_to_next_level();
	unsigned long ulExpRemaining = ulExpRequired - ulCurrentExp;

	ImGui::Text("Name: %s", szName.c_str());

	ImGui::Text("Elite Pilot, Level %d", iCurrentLevel);

	if(this->m_pEntity->get_level() == 400)
	{
		ImGui::Text("Max Level (congrats, now go get a life)");
	}
	else
	{
		ImGui::Text("Current EXP: %lu", ulCurrentExp);
		ImGui::Text("EXP required to achieve level %d: %lu", iCurrentLevel + 1, ulExpRequired);
		ImGui::Text("EXP remaining: %lu", ulExpRemaining);
	}

	if(ImGui::Button("Done"))
	{
		this->m_bPanelActive = false;
	}

	ImGui::End();
}

bool CharacterSheetPanel::panel_active(void)
{
	return this->m_bPanelActive;
}