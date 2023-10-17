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
#include "AsteroidProspectPanel.hxx"
#include "LoadoutPanel.hxx"
#include "SGLib.hxx"
#include <sstream>
#include "MaterialExaminePanel.hxx"

#define LINE_LENGTH 24
#define LINES_PER_BOX 12

int AsteroidProspectPanel::m_iPanelInstances = 0;

void AsteroidProspectPanel::render_panel(float const flDelta)
{
	if(this->m_uiAstfieldId == 0)
	{
		return;
	}

	ImGui::Begin("Prospect Asteroid");

	ImGui::Text(this->m_szIdText.c_str());

	ImGui::Separator();

	ImGui::Columns(2, "items1", true);  // 3-ways, no border

	//ImGui::Text("Asteroid Field Description:");
	ImVec2 BufSize = ImGui::CalcTextSize(this->m_szDescBuffer);
	ImGui::InputTextMultiline("##desc", this->m_szDescBuffer, BUFFER_SIZE, ImVec2(BufSize.x + 16.0f, ImGui::GetTextLineHeight() * LINES_PER_BOX), ImGuiInputTextFlags_ReadOnly);

	ImGui::NextColumn();

	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
	ImGui::BeginChild("Sub1", ImVec2(ImGui::GetWindowContentRegionWidth(), 150.0f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Deposits");

	AsteroidProspectPanel::do_mat_list(this->m_matQuantityMap);

	ImGui::EndChild();
	ImGui::PopStyleVar(1);

	ImGui::Columns(1);

	if(ImGui::Button("Done"))
	{
		this->m_bPanelActive = false;
	}

	ImGui::End();
}

bool AsteroidProspectPanel::panel_active(void)
{
	return this->m_bPanelActive;
}

void AsteroidProspectPanel::build_desc_text_block(void)
{
	CAstfield const& astfield = SG::get_universe()->get_astfield(this->m_uiAstfieldId);

	/*
	* Short snippet to add line breaks to description
	*/
	std::string str("");
	std::string szDesc(astfield.get_desc().c_str());

	bool bCancelLine = false;

	for(int i = 0; i < astfield.get_desc().size(); ++i)
	{
		if(i && i % LINE_LENGTH == 0)
		{
			bCancelLine = true;
		}

		if(bCancelLine && szDesc[i] == ' ')
		{
			str.push_back('\n');
			bCancelLine = false;
		}
		else
		{
			str.push_back(szDesc[i]);
		}
	}

	strcpy_s(this->m_szDescBuffer, BUFFER_SIZE, str.c_str());

	IMaterial* pMaterial = SG::get_material_manager()->get_material_association(this->m_uiAstfieldId);

	if(pMaterial != nullptr)
	{
		this->m_szDepositSerial = pMaterial->get_serial_number();

		this->m_matQuantityMap[this->m_szDepositSerial] = 1;
	}
}

void AsteroidProspectPanel::build_id_text_block(void)
{
	CAstfield const& astfield = SG::get_universe()->get_astfield(this->m_uiAstfieldId);
	MaterialType matType = astfield.get_mat_type();
	std::string matTypeString = IMaterialClass::get_material_type_name(matType);

	unsigned int uiAvgDepositSize = (astfield.get_min_drop_quantity() + astfield.get_max_drop_quantity()) / 2;

	std::stringstream ss("");

	ss << astfield.get_name() << std::endl;
	ss << "Deposit Type: " << matTypeString << std::endl;
	ss << "Avg Deposit Size: " << uiAvgDepositSize << std::endl;

	this->m_szIdText = ss.str();
}

void AsteroidProspectPanel::do_mat_list(MaterialQuantityMap& itemList)
{
	std::stringstream ss;

	for(MaterialQuantityMap::value_type& item : itemList)
	{
		if(item.first.size() && item.second > 0)
		{
			IMaterial* pMaterial = SG::get_material_manager()->get_material_from_serial(item.first);

			if(pMaterial)
			{
				ss.clear();
				ss.str(std::string());
				ss << pMaterial->get_name() << std::endl;

				//ss << " (" << item.second << ")" << std::endl;

				if(ImGui::Selectable(ss.str().c_str()))
				{

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
			}
		}
		else
		{
			ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "Nothing.");
		}
	}
}