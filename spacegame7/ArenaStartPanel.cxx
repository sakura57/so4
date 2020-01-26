#pragma once

#include "SGLib.hxx"
#include "ICharacterEntity.hxx"
#include "ArenaStartPanel.hxx"
#include "CSectorTransitionState.hxx"

bool ArenaStartPanel::m_bPanelExists = false;

void ArenaStartPanel::render_panel(float const flDelta)
{
	if (!this->m_bPanelExists)
	{
		return;
	}

	static char szCharName[16] = "Recruit";
	static int iCurrentItem = 0;

	ImGui::SetNextWindowPosCenter(ImGuiCond_Once | ImGuiCond_Appearing);

	ImVec2 displaySize = ImGui::GetIO().DisplaySize;

	ImVec2 windowSize(displaySize.x / 8.0f, displaySize.y / 4.0f);

	ImGui::Begin(
		"Arena Start", nullptr, windowSize, -1.0F,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	ImGui::InputText("Name", szCharName, 16, ImGuiInputTextFlags_CharsNoBlank);

	ImGui::Combo("Loadout", &iCurrentItem, "Hawk\0Banshee\0Saber\0\0");

	if (ImGui::Button("Begin"))
	{
		ICharacterEntityManager* pEntityManager = SG::get_intransient_data_manager()->get_character_entity_manager();
		pEntityManager->create_player_entity();

		ICharacterEntity* pEntity = pEntityManager->get_player_character_entity();
		IEntityInventory* pInventory = pEntity->get_inventory();

		pInventory->set_money(1000);
		pInventory->set_metal(0);
		pInventory->clear_items();

		pEntity->set_name(std::string(szCharName));

		switch (iCurrentItem)
		{
		case 0:
			pEntity->set_level(1);
			pEntity->set_loadout({ 8, 8, 8, 8, 15, 0 });
			pEntity->set_ship(1);
			break;
		case 1:
			pEntity->set_level(1);
			pEntity->set_loadout({ 8, 8, 8, 15 });
			pEntity->set_ship(3);
			break;
		case 2:
			pEntity->set_level(1);
			pEntity->set_loadout({ 8, 8, 8, 8, 15, 0 });
			pEntity->set_ship(11);
			break;
		}

		SG::get_game_state_manager()->transition_game_state(new CSectorTransitionState(7, "", Vector2f(0.0f, 0.0f)));
	}

	ImGui::End();
}

bool ArenaStartPanel::panel_active(void)
{
	return this->m_bPanelActive;
}