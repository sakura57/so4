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
#include "EquipTraderPanel.hxx"
#include "LoadoutPanel.hxx"
#include "SGLib.hxx"
#include <sstream>

int EquipTraderPanel::m_iPanelInstances = 0;

void EquipTraderPanel::render_panel(float const flDelta)
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

	ImGui::Begin("Equipment Trader");

	ImGui::Text("Money: %d", this->m_pInventory->get_money());
	ImGui::Text("Metal: %d", this->m_pInventory->get_metal());

	ImGui::Separator();

	ImGui::TextWrapped("Welcome to the Equipment Trader. You can buy and sell equipment here, as well as outfit your ship for combat.\nRight-click an item to perform an action on it.");

	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
	ImGui::BeginChild("Sub1", ImVec2(ImGui::GetWindowContentRegionWidth(), 200.0f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::Columns(3, "items1", true);  // 3-ways, no border

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Trader");

	if(!this->m_vEquipmentSelling.size())
	{
		ImGui::Text("Empty.");
	}
	else
	{
		sk.clear();
		sk.str("");
		sk << "t";
		
		ICharacterEntity *pPlayerEntity = SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity();

		for(ArchId const arch : this->m_vEquipmentSelling)
		{
			sk << arch;
			sk << "#";

			Archetype const *pArch = SG::get_game_data_manager()->get_arch<Archetype>(arch);
			
			float flPriceModifier = 1.0f;

			flPriceModifier -= pPlayerEntity->get_stat(Stat::NEGOTIATION) * 0.01f;

			if (flPriceModifier < 0.0f)
			{
				flPriceModifier = 0.0f;
			}

			int iAdjustedPrice = (int)(flPriceModifier * pArch->iValue);

			ss.clear();
			ss.str(std::string());
			ss << pArch->get_name();

			if(ImGui::Selectable(ss.str().c_str()))
			{

			}

			if(ImGui::IsItemHovered())
			{
				LoadoutPanel::do_item_tooltip(reinterpret_cast<EquipArch const*>(pArch));
			}

			if(ImGui::BeginPopupContextItem(sk.str().c_str()))
			{
				ImGui::Text("Price: %d", iAdjustedPrice);

				ImGui::Separator();

				if(ImGui::Selectable("Buy & Mount"))
				{
					//Buy an item and mount it
					if(this->m_pInventory->get_money() >= iAdjustedPrice)
					{
						for(int i = 0; i < lLoadout.size(); ++i)
						{
							if(lLoadout[i] == 0U)
							{
								this->m_pInventory->adjust_money(-iAdjustedPrice);

								lLoadout[i] = arch;

								pPlayerEntity->set_loadout(lLoadout);

								SG::get_audio_manager()->play_sound(27U);

								break;
							}
						}
					}
				}

				if(ImGui::Selectable("Buy"))
				{
					//Buy an item
					if(this->m_pInventory->get_money() >= iAdjustedPrice)
					{
						this->m_pInventory->adjust_money(-iAdjustedPrice);

						this->m_pInventory->add_item(arch, 1);

						SG::get_audio_manager()->play_sound(26U);
					}
				}

				ImGui::EndPopup();
			}
		}
	}

	ImGui::NextColumn();

	itemList.clear();
	this->m_pInventory->get_items_in_inventory(itemList);

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Cargo Hold");

	if(!itemList.size())
	{
		ImGui::Text("Empty.");
	}
	else
	{
		sk.clear();
		sk.str("");
		sk << "c";

		for(ItemList::value_type &item : itemList)
		{
			sk << item.first;
			sk << "#";

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

			if(ImGui::BeginPopupContextItem(sk.str().c_str()))
			{
				float flPriceModifier = 1.0f;

				flPriceModifier += pPlayerEntity->get_stat(Stat::NEGOTIATION) * 0.01f;

				int iFinalSellPrice = (int)(flPriceModifier * pArch->iValue) / 2;

				ImGui::Text("Sell Value: %d", iFinalSellPrice);

				ImGui::Separator();

				if(ImGui::Selectable("Mount"))
				{
					//Mount an item from the cargo hold
					for(int i = 0; i < lLoadout.size(); ++i)
					{
						if(lLoadout[i] == 0U)
						{
							lLoadout[i] = item.first;
							this->m_pInventory->remove_item(item.first, 1);

							pPlayerEntity->set_loadout(lLoadout);

							SG::get_audio_manager()->play_sound(27U);

							break;
						}
					}
				}

				if(ImGui::Selectable("Sell"))
				{
					//Sell an item in the cargo hold
					this->m_pInventory->adjust_money(iFinalSellPrice);
					this->m_pInventory->remove_item(item.first, 1);

					//coin sound effect
					SG::get_audio_manager()->play_sound(13);
				}

				ImGui::EndPopup();
			}
		}
	}

	ImGui::NextColumn();

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Equipped");

	ItemList loadoutAsList;

	for(ArchId arch : lLoadout)
	{
		loadoutAsList.push_back(std::pair<ArchId, unsigned>(arch, 1));
	}

	sk.clear();
	sk.str("");

	sk << "l";

	int iLoadoutPosition = 0;

	for(ItemList::value_type &item : loadoutAsList)
	{
		sk << item.first;
		sk << "#";

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
				//dummy selectable
			}

			if(ImGui::IsItemHovered())
			{
				LoadoutPanel::do_item_tooltip(reinterpret_cast<EquipArch const*>(pArch));
			}

			if(ImGui::BeginPopupContextItem(sk.str().c_str()))
			{
				float flPriceModifier = 1.0f;

				flPriceModifier += pPlayerEntity->get_stat(Stat::NEGOTIATION) * 0.01f;

				int iFinalSellPrice = (int)(flPriceModifier * pArch->iValue) / 2;

				ImGui::Text("Sell Value: %d", iFinalSellPrice);

				ImGui::Separator();

				if(ImGui::Selectable("Demount"))
				{
					//demount an equipped item
					this->m_pInventory->add_item(item.first, 1);
					pPlayerEntity->demount_item(iLoadoutPosition);

					SG::get_audio_manager()->play_sound(28U);
				}

				if(ImGui::Selectable("Sell"))
				{
					//sell an equipped item
					this->m_pInventory->adjust_money(iFinalSellPrice);
					pPlayerEntity->demount_item(iLoadoutPosition);

					//coin sound effect
					SG::get_audio_manager()->play_sound(13U);
				}

				ImGui::EndPopup();
			}
		}
		else
		{
			ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "Nothing");
		}

		++iLoadoutPosition;
	}

	ImGui::EndChild();
	ImGui::PopStyleVar(1);

	if(ImGui::Button("Leave"))
	{
		this->m_bPanelActive = false;
	}

	ImGui::End();
}

bool EquipTraderPanel::panel_active(void)
{
	return this->m_bPanelActive;
}