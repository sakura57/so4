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
			SG::get_game_data_manager()->load_from_save(saveFileName);
		}
		catch (SGException e)
		{
			SG::get_game_state_manager()->get_game_state()->state_send_notification("Could not load save");
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

	ImGui::End();
};

bool PlayerDiedPanel::panel_active(void)
{
	return true;
};