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