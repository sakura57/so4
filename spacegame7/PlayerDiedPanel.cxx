#include "SGLib.hxx"
#include "CMainMenuState.hxx"
#include "PlayerDiedPanel.hxx"

bool PlayerDiedPanel::m_bPanelExists = false;

void PlayerDiedPanel::render_panel(float const flDelta)
{
	ImGui::SetNextWindowPosCenter(ImGuiCond_Once | ImGuiCond_Appearing);

	ImGui::Begin(
		"You are dead",
		nullptr,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings
	);

	ImGui::Text("You have died.");

	ImGui::Separator();


	ImGui::TextWrapped(this->m_szDeathString.c_str());


	ImGui::Separator();

	if(ImGui::Button("Load Game"))
	{
		std::string saveFileName = ".\\saves\\";
		saveFileName.append(SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity()->get_name());
		saveFileName.append(".sav");

		try
		{
			this->m_bGameLoadAttempt = SG::get_game_data_manager()->load_from_save(saveFileName);
		}
		catch (SGException e)
		{
			SG::get_game_state_manager()->get_game_state()->state_send_notification("Could not load save");
		}
	}

	if(this->m_bGameLoadAttempt.valid())
	{
		if(this->m_bGameLoadAttempt.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			if(this->m_bGameLoadAttempt.get() == true)
			{
				//the load operation succeeded.
				//in practice, this piece of code will probably never be reached,
				//because the interface manager will be shifted out as a result of the
				//load operation, but just in case, we will delete ourself.

				this->m_bPanelActive = false;
			}
			else
			{
				ImGui::OpenPopup("Load Failed");
			}
		}
	}

	ImGui::SameLine();

	if(ImGui::Button("Exit to Menu"))
	{
		SG::get_game_state_manager()->transition_game_state(new CMainMenuState);
	}

	ImGui::SameLine();

	if(ImGui::Button("Exit to Desktop"))
	{
		//TODO: make this do something
	}

	if(ImGui::BeginPopupModal("Load Failed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Load game failed.\nThe savegame does not exist or is invalid.");
		ImGui::Separator();

		if(ImGui::Button("Okay", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::End();
};

bool PlayerDiedPanel::panel_active(void)
{
	return true;
};