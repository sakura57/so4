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
#include "MaterialTraderPanel.hxx"
#include "MaterialExaminePanel.hxx"
#include "LoadoutPanel.hxx"
#include "SGLib.hxx"
#include <sstream>

int MaterialTraderPanel::m_iPanelInstances = 0;

void MaterialTraderPanel::render_panel(float const flDelta)
{
	if(this->m_pMaterialBank == nullptr)
	{
		return;
	}

	static IMarketData *pMarketData = SG::get_market_manager()->get_market_data(this->m_iBaseId);

	//std::vector<std::string> items;
	ICharacterEntity* pPlayerEntity = SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity();

	Loadout lLoadout = pPlayerEntity->get_loadout();

	ItemList itemList;
	std::stringstream ss;
	std::stringstream sk;

	ImGui::Begin("Material Trader", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);

	ImGui::TextWrapped("Welcome to the Materials Trader. Here you can sell materials obtained through asteroid mining.");

	ImGui::Separator();

	ImGui::Text("Money: %d", this->m_pInventory->get_money());

	static int buysellamt = 0;

	ImGui::Text("Sell Amount");
	ImGui::InputInt("", &buysellamt, 1, 100);

	if(buysellamt < 0)
	{
		buysellamt = 0;
	}

	if(this->m_pSelectedMaterial != nullptr)
	{
		bool cansell = (this->m_pMaterialBank->get_material_quantity(this->m_pSelectedMaterial->get_serial_number()) >= (unsigned int)buysellamt);
		int sellprice = 0;

		if(cansell)
		{
			sellprice = (int)pMarketData->get_material_price(this->m_pSelectedMaterial, (unsigned int)buysellamt);

			ImGui::Text("Price for %d: %d", buysellamt, sellprice);
		}
		else
		{
			ImGui::Text("You don't have enough!");
		}

		if(ImGui::Button("Sell"))
		{
			if(cansell)
			{
				unsigned int uiPreviousQuantity = this->m_pMaterialBank->get_material_quantity(this->m_pSelectedMaterial->get_serial_number());
				this->m_pMaterialBank->set_material_quantity(this->m_pSelectedMaterial->get_serial_number(), uiPreviousQuantity - buysellamt);

				this->m_pInventory->adjust_money(sellprice);

				//coin sound effect
				SG::get_audio_manager()->play_sound(13);
			}
		}
	}
	else
	{
		ImGui::Text("No material selected.");
	}

	ImGui::Separator();

	MaterialQuantityMap matQuantityMap;
	this->m_pMaterialBank->get_material_quantity_map(matQuantityMap);

	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
	ImGui::BeginChild("Sub1", ImVec2(ImGui::GetWindowContentRegionWidth(), 150.0f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	//ImGui::Columns(2, "items1", true);  // 3-ways, no border

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Materials");

	if(matQuantityMap.size() == 0)
	{
		ImGui::Text("Empty.");
	}
	else
	{
		MaterialTraderPanel::do_mat_list(matQuantityMap);
	}

	//ImGui::NextColumn();
	ImGui::EndChild();
	ImGui::PopStyleVar(1);

	if(ImGui::Button("Leave"))
	{
		this->m_bPanelActive = false;
	}

	ImGui::End();
}

bool MaterialTraderPanel::panel_active(void)
{
	return this->m_bPanelActive;
}

void MaterialTraderPanel::do_mat_list(MaterialQuantityMap& itemList)
{
	std::stringstream ss;

	unsigned int totalMaterials = 0;

	for(MaterialQuantityMap::value_type& item : itemList)
	{
		if(item.first.size() && item.second > 0)
		{
			IMaterial* pMaterial = SG::get_material_manager()->get_material_from_serial(item.first);

			if(pMaterial)
			{
				ss.clear();
				ss.str(std::string());
				ss << pMaterial->get_name();

				ss << " (" << item.second << ")" << std::endl;

				if(ImGui::Selectable(ss.str().c_str()))
				{
					this->m_pSelectedMaterial = pMaterial;
				}

				std::string sk = "POPUP#" + pMaterial->get_serial_number();

				if(ImGui::BeginPopupContextItem(sk.c_str()))
				{
					if(ImGui::Selectable("Examine"))
					{
						SG::get_interface_manager()->add_panel(new MaterialExaminePanel(pMaterial->get_serial_number()));
					}

					ImGui::EndPopup();
				}

				if(ImGui::IsItemHovered())
				{
					//TODO: make tooltip
				}

				totalMaterials += item.second;
			}
		}
	}

	if(totalMaterials = 0)
	{
		ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "Nothing.");
	}
}