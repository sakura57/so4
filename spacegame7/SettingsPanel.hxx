#pragma once

#include <utility>
#include "InterfacePanel.hxx"

typedef std::vector<std::pair<unsigned int, unsigned int>> ResolutionList;
typedef std::vector<std::string> ResolutionStringList;

class SettingsPanel : public InterfacePanel
{
public:
	SettingsPanel() : m_bPopupActive(true)
	{};
	virtual ~SettingsPanel()
	{};

	virtual void render_panel(float const flDelta)
	{
		if(!this->m_bPopupActive)
		{
			return;
		}

		auto settings = SG::get_game_settings();

		ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

		ImGui::Text("Audio");
		ImGui::Separator();
		{
			float soundVolume = sf::Listener::getGlobalVolume();

			if(ImGui::SliderFloat("Sound Volume", &soundVolume, 0.0f, 100.0f, "%.0f"))
			{
				sf::Listener::setGlobalVolume(soundVolume);
			}

			static float musicVolume = 0.0f;

			if (ImGui::SliderFloat("Music Volume", &musicVolume, 0.0f, 100.0f, "%.0f"))
			{
				//TODO: make music volume do something
			}
		}
		

		ImGui::Text("Graphics");
		ImGui::Separator();
		{
			bool bOpenChangeDialog = false;

			unsigned int fullscreen = 0;
			settings->get_setting<unsigned int>("fullscreen", fullscreen);
			bool bFullscreenChecked = fullscreen != 0;
			bool bNewFullscreenChecked = bFullscreenChecked;

			ImGui::Checkbox("Fullscreen", &bNewFullscreenChecked);

			if (bNewFullscreenChecked != bFullscreenChecked)
			{
				settings->set_setting<unsigned int>("fullscreen", bNewFullscreenChecked ? 1 : 0);
				bFullscreenChecked = bNewFullscreenChecked;

				bOpenChangeDialog = true;
			}

			ResolutionList resList;
			ResolutionStringList resStrList;

			this->get_resolutions(resList, resStrList);

			unsigned int screen_width = 0;
			settings->get_setting<unsigned int>("screen_width", screen_width);

			unsigned int screen_height = 0;
			settings->get_setting<unsigned int>("screen_height", screen_height);

			int resSlot = 0;

			for (resSlot = 0; resSlot < resList.size(); ++resSlot)
			{
				if (resList[resSlot].first == screen_width && resList[resSlot].second)
				{
					break;
				}
			}

			if (resSlot == resList.size())
			{
				resList.push_back(std::pair<unsigned int, unsigned int>(screen_width, screen_height));
				resStrList.push_back("Custom");
			}

			if (ImGui::BeginCombo("Resolution", resStrList[resSlot].c_str()))
			{
				for (unsigned int i = 0; i < 5; ++i)
				{
					if (ImGui::Selectable(resStrList[i].c_str()))
					{
						settings->set_setting<unsigned int>("screen_width", resList[i].first);
						settings->set_setting<unsigned int>("screen_height", resList[i].second);
						bOpenChangeDialog = true;
					}
				}

				ImGui::EndCombo();
			}

			char const qualityLevels[][32] =
			{
				"Very Low",
				"Low",
				"Medium",
				"High",
				"Very High"
			};

			unsigned int qualityLevel = 0;
			settings->get_setting<unsigned int>("quality_level", qualityLevel);

			if (ImGui::BeginCombo("Graphics Quality", qualityLevels[qualityLevel]))
			{
				for (unsigned int i = 0; i < 5; ++i)
				{
					if (ImGui::Selectable(qualityLevels[i]))
					{
						qualityLevel = i;
						settings->set_setting("quality_level", i);
					}
				}

				ImGui::EndCombo();
			}

			ImGui::Text("Controls");
			ImGui::Separator();

			char const controlsOptions[][32] =
			{
				"Keyboard Only",
				"Keyboard & Mouse",
			};

			unsigned int controlStyle = 0;
			settings->get_setting<unsigned int>("controls_style", controlStyle);

			if (ImGui::BeginCombo("Controls Style", controlsOptions[controlStyle]))
			{
				for (unsigned int i = 0; i < 2; ++i)
				{
					if (ImGui::Selectable(controlsOptions[i]))
					{
						controlStyle = i;
						settings->set_setting<unsigned int>("controls_style", i);
					}
				}

				ImGui::EndCombo();
			}

			if (ImGui::Button("Close"))
			{
				this->m_bPopupActive = false;
			}

			if (bOpenChangeDialog)
			{
				this->open_next_start_popup();
			}

			this->do_next_start_popup();
		}

		ImGui::End();
	};

	virtual bool panel_active(void)
	{
		return this->m_bPopupActive;
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
};