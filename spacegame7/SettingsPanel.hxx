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

#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include "SFMLIncludes.hxx"
#include "InterfacePanel.hxx"

typedef std::vector<std::pair<unsigned int, unsigned int>> ResolutionList;
typedef std::vector<std::string> ResolutionStringList;

class SettingsPanel : public InterfacePanel
{
public:
	SettingsPanel();
	virtual ~SettingsPanel();

	virtual void render_panel(float const);

	virtual bool panel_active(void)
	{
		return this->m_bPanelActive;
	};

	virtual bool prevents_game_input(void)
	{
		return true;
	};
private:
	void get_resolutions(ResolutionList &resList, ResolutionStringList &resStrList)
	{
		static bool gotten_resolutions = false;
		static ResolutionList sResList;
		static ResolutionStringList sResStrList;

		if(!gotten_resolutions)
		{
			std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

			for(auto mode : modes)
			{
				std::stringstream ss;
				ss << mode.width << "x" << mode.height;
				sResList.push_back(std::pair<unsigned int, unsigned int>(mode.width, mode.height));
				sResStrList.push_back(ss.str());
			}

			gotten_resolutions = true;
		}

		resList = sResList;
		resStrList = sResStrList;
	};

	void open_next_start_popup(void)
	{
		ImGui::OpenPopup("Need Restart");
	};

	void do_next_start_popup(void)
	{
		if (ImGui::BeginPopupModal("Need Restart", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("This setting change will not take effect\nuntil the next time the game is started.\n\n");
			ImGui::Separator();

			if (ImGui::Button("Okay", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}
	};

	bool m_bPopupActive;

	bool m_bPanelActive;
	static int m_iPanelInstances;
};