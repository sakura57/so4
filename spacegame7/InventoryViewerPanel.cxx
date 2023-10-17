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
#include "InventoryViewerPanel.hxx"
#include "LoadoutPanel.hxx"
#include "SGLib.hxx"
#include <sstream>

int InventoryViewerPanel::m_iPanelInstances = 0;

void InventoryViewerPanel::render_panel(float const flDelta)
{
	if(this->m_pInventory == nullptr)
	{
		return;
	}

	//std::vector<std::string> items;
	ItemList itemList;
	this->m_pInventory->get_items_in_inventory(itemList);

	ImGui::Begin("Inventory");

	ImGui::Text("Money: %d", this->m_pInventory->get_money());
	ImGui::Text("Metal: %d", this->m_pInventory->get_metal());

	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
	ImGui::BeginChild("Sub1", ImVec2(ImGui::GetWindowContentRegionWidth(), 150.0f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::Columns(2, "items1", true);  // 3-ways, no border
	//ImGui::Separator();

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Cargo Hold");

	if(!itemList.size())
	{
		ImGui::Text("Empty.");
	}
	else
	{
		InventoryViewerPanel::do_item_list(itemList);
	}

	ImGui::NextColumn();

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Equipped");

	ICharacterEntity *pPlayerEntity = SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity();

	Loadout lLoadout = pPlayerEntity->get_loadout();

	ItemList loadoutAsList;

	for(ArchId arch : lLoadout)
	{
		loadoutAsList.push_back(std::pair<ArchId, unsigned>(arch, 1));
	}

	InventoryViewerPanel::do_item_list(loadoutAsList);

	ImGui::NextColumn();
	ImGui::EndChild();
	ImGui::PopStyleVar(1);

	if(ImGui::Button("Done"))
	{
		this->m_bPanelActive = false;
	}

	ImGui::End();
}

bool InventoryViewerPanel::panel_active(void)
{
	return this->m_bPanelActive;
}

void InventoryViewerPanel::do_item_list(ItemList &itemList)
{
	std::stringstream ss;

	for(ItemList::value_type &item : itemList)
	{
		if(item.first)
		{
			Archetype const *pArch = SG::get_game_data_manager()->get_arch<Archetype>(item.first);
			ss.clear();
			ss.str(std::string());
			ss << pArch->get_name();

			if(item.second > 1)
			{
				ss << " (" << item.second << ")" << std::endl;
			}

			if(ImGui::Selectable(ss.str().c_str()))
			{

			}

			if(ImGui::IsItemHovered())
			{
				LoadoutPanel::do_item_tooltip(reinterpret_cast<EquipArch const*>(pArch));
			}
		}
		else
		{
			ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "Nothing");
		}
	}
}