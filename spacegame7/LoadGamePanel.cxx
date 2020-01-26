#pragma once

#include "SGLib.hxx"
#include "ICharacterEntity.hxx"
#include "LoadGamePanel.hxx"
#include "CSectorTransitionState.hxx"

bool LoadGamePanel::m_bPanelExists = false;

void LoadGamePanel::render_panel(float const flDelta)
{
	if (!this->m_bPanelExists)
	{
		return;
	}

	ImGui::SetNextWindowPosCenter(ImGuiCond_Once | ImGuiCond_Appearing);

	ImVec2 displaySize = ImGui::GetIO().DisplaySize;

	ImVec2 windowSize(displaySize.x / 8.0f, displaySize.y / 4.0f);

	ImGui::Begin(
		"Load Game", nullptr, windowSize, -1.0F,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	static char szCharName[16] = "Recruit";

	ImGui::InputText("Name", szCharName, 16, ImGuiInputTextFlags_CharsNoBlank);

	if (ImGui::Button("Load"))
	{
		std::stringstream ss;
		ss << "saves\\";
		ss << szCharName;
		ss << ".sav";
		SG::get_game_data_manager()->load_from_save(ss.str());
	}

	ImGui::End();
}

bool LoadGamePanel::panel_active(void)
{
	return this->m_bPanelActive;
}