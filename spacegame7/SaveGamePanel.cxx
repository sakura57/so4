#include "SaveGamePanel.hxx"
#include "LoadoutPanel.hxx"
#include "SGLib.hxx"
#include <sstream>

int SaveGamePanel::m_iPanelInstances = 0;

void SaveGamePanel::render_panel(float const flDelta)
{
	ImGui::SetNextWindowPosCenter(ImGuiCond_Once | ImGuiCond_Appearing);

	ImVec2 displaySize = ImGui::GetIO().DisplaySize;

	ImVec2 windowSize(displaySize.x / 8.0f, displaySize.y / 4.0f);

	ImGui::Begin(
		"Save Game", nullptr, windowSize, -1.0F,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	if (ImGui::Button("Save"))
	{
		//SG::get_game_data_manager()->dump_to_save("saves\\testsave.sav", 1U);
	}

	ImGui::End();
}

bool SaveGamePanel::panel_active(void)
{
	return this->m_bPanelActive;
}