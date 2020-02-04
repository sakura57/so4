#include "SettingsPanel.hxx"
#include "SGLib.hxx"

SettingsPanel::SettingsPanel()
	: m_bPopupActive(true)
{
}

SettingsPanel::~SettingsPanel()
{
}

void SettingsPanel::render_panel(float const flDelta)
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
		float soundVolume;

		settings->get_setting<float>("sound_volume", soundVolume);

		if(ImGui::SliderFloat("Sound Volume", &soundVolume, 0.0f, 100.0f, "%.0f"))
		{
			settings->set_setting<float>("sound_volume", soundVolume);

			sf::Listener::setGlobalVolume(soundVolume);
		}

		float musicVolume;

		settings->get_setting<float>("music_volume", musicVolume);

		if(ImGui::SliderFloat("Music Volume", &musicVolume, 0.0f, 100.0f, "%.0f"))
		{
			settings->set_setting<float>("music_volume", musicVolume);

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

		if(bNewFullscreenChecked != bFullscreenChecked)
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

		for(resSlot = 0; resSlot < resList.size(); ++resSlot)
		{
			if(resList[resSlot].first == screen_width && resList[resSlot].second)
			{
				break;
			}
		}

		if(resSlot == resList.size())
		{
			resList.push_back(std::pair<unsigned int, unsigned int>(screen_width, screen_height));
			resStrList.push_back("Custom");
		}

		if(ImGui::BeginCombo("Resolution", resStrList[resSlot].c_str()))
		{
			for(unsigned int i = 0; i < 5; ++i)
			{
				if(ImGui::Selectable(resStrList[i].c_str()))
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

		if(ImGui::BeginCombo("Graphics Quality", qualityLevels[qualityLevel]))
		{
			for(unsigned int i = 0; i < 5; ++i)
			{
				if(ImGui::Selectable(qualityLevels[i]))
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

		if(ImGui::BeginCombo("Controls Style", controlsOptions[controlStyle]))
		{
			for(unsigned int i = 0; i < 2; ++i)
			{
				if(ImGui::Selectable(controlsOptions[i]))
				{
					controlStyle = i;
					settings->set_setting<unsigned int>("controls_style", i);
				}
			}

			ImGui::EndCombo();
		}

		if(ImGui::Button("Close"))
		{
			this->m_bPopupActive = false;
		}

		if(bOpenChangeDialog)
		{
			this->open_next_start_popup();
		}

		this->do_next_start_popup();
	}

	ImGui::End();
}