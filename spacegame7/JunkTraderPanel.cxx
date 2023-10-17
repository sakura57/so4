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
#include "JunkTraderPanel.hxx"
#include "LoadoutPanel.hxx"
#include "SGLib.hxx"
#include <sstream>

int JunkTraderPanel::m_iPanelInstances = 0;

void JunkTraderPanel::render_panel(float const flDelta)
{
	if(this->m_pInventory == nullptr)
	{
		return;
	}

	//std::vector<std::string> items;
	ICharacterEntity *pPlayerEntity = SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity();

	Loadout lLoadout = pPlayerEntity->get_loadout();

	ItemList itemList;
	std::stringstream ss;
	std::stringstream sk;

	ImGui::Begin("Junk Trader", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);

	ImGui::Text("Money: %d", this->m_pInventory->get_money());
	ImGui::Text("Metal: %d", this->m_pInventory->get_metal());

	ImGui::Separator();

	float flSellPriceMultiplier = 0.5f;
	float flBuyPriceMultiplier = 1.0f;

	flBuyPriceMultiplier -= pPlayerEntity->get_stat(Stat::NEGOTIATION) * 0.01f;
	flSellPriceMultiplier += pPlayerEntity->get_stat(Stat::NEGOTIATION) * 0.01f;

	if (flBuyPriceMultiplier < 0.0f)
	{
		flBuyPriceMultiplier = 0.0f;
	}

	int iSellPrice = flSellPriceMultiplier * this->m_iExchangeRate;
	int iBuyPrice = flBuyPriceMultiplier * this->m_iExchangeRate;

	ImGui::Text("Base Rate: %d credits per unit", this->m_iExchangeRate);
	ImGui::Text("Final Buy Rate: %d credits per unit", iBuyPrice);
	ImGui::Text("Final Sell Rate: %d credits per unit", iSellPrice);

	ImGui::Separator();

	static int buysellmetal = 0;

	ImGui::Text("Scrap Metal");
	ImGui::InputInt("", &buysellmetal, 1, 100);

	if(ImGui::Button("Buy"))
	{
		if(buysellmetal > 0)
		{
			int iCost = buysellmetal * iBuyPrice;

			if(this->m_pInventory->get_money() >= iCost)
			{
				this->m_pInventory->adjust_metal(buysellmetal);
				this->m_pInventory->adjust_money(-iCost);

				//coin sound effect
				SG::get_audio_manager()->play_sound(13);
			}
		}
	}

	ImGui::SameLine();
	
	if(ImGui::Button("Sell"))
	{
		if(buysellmetal > 0)
		{
			int iCost = buysellmetal * iSellPrice;

			if(this->m_pInventory->get_metal() >= buysellmetal)
			{
				this->m_pInventory->adjust_metal(-buysellmetal);
				this->m_pInventory->adjust_money(iCost);

				//coin sound effect
				SG::get_audio_manager()->play_sound(13);
			}
		}
	}

	ImGui::Separator();

	if(ImGui::Button("Leave"))
	{
		this->m_bPanelActive = false;
	}

	ImGui::End();
}

bool JunkTraderPanel::panel_active(void)
{
	return this->m_bPanelActive;
}