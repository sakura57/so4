#include "MainMenuPanel.hxx"
#include "CGameExitState.hxx"

void MainMenuPanel::render_panel(float const flDelta)
{
	ImGui::SetNextWindowPosCenter(ImGuiCond_Once | ImGuiCond_Appearing);

	ImVec2 displaySize = ImGui::GetIO().DisplaySize;

	ImVec2 windowSize(displaySize.x / 8.0f, displaySize.y / 4.0f);

	ImGui::Begin(
		"Main Menu", nullptr, windowSize, -1.0F,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	float windowWidth = ImGui::GetWindowWidth();

	ImVec2 buttonSize(windowWidth - windowWidth / 4.0f, 0.0f);
	ImGui::SetCursorPosX(windowWidth / 8.0f);
	if(ImGui::Button("Story - New", buttonSize))
	{
		this->m_bPanelActive = false;

		SG::get_interface_manager()->add_panel(new CharacterCreationPanel);
	}

	ImGui::SetCursorPosX(windowWidth / 8.0f);
	if(ImGui::Button("Story - Load", buttonSize))
	{
		this->m_bPanelActive = false;

		SG::get_interface_manager()->add_panel(new LoadGamePanel);
	}

	ImGui::SetCursorPosX(windowWidth / 8.0f);
	if(ImGui::Button("Arena", buttonSize))
	{
		this->m_bPanelActive = false;

		SG::get_interface_manager()->add_panel(new ArenaStartPanel);
	}

	ImGui::SetCursorPosX(windowWidth / 8.0f);
	if(ImGui::Button("Options", buttonSize))
	{
		SG::get_interface_manager()->add_panel(new SettingsPanel);
	}

	ImGui::SetCursorPosX(windowWidth / 8.0f);
	if(ImGui::Button("Exit", buttonSize))
	{
		SG::get_game_state_manager()->transition_game_state(new CGameExitState);
	}

	ImGui::End();
}