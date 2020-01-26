#include "MaterialBankPanel.hxx"
#include "LoadoutPanel.hxx"
#include "SGLib.hxx"
#include <sstream>
#include "MaterialExaminePanel.hxx"

bool MaterialBankPanel::m_bPanelExists = false;

void MaterialBankPanel::render_panel(float const flDelta)
{
	if(!this->m_bPanelExists)
	{
		return;
	}

	if(this->m_pMatBank == nullptr)
	{
		return;
	}

	//std::vector<std::string> items;
	MaterialQuantityMap matQuantityMap;
	this->m_pMatBank->get_material_quantity_map(matQuantityMap);

	ImGui::Begin("Materials");

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
		MaterialBankPanel::do_mat_list(matQuantityMap);
	}

	//ImGui::NextColumn();
	ImGui::EndChild();
	ImGui::PopStyleVar(1);

	if(ImGui::Button("Done"))
	{
		this->m_bPanelExists = false;
		this->m_bPanelActive = false;
	}

	ImGui::End();
}

bool MaterialBankPanel::panel_active(void)
{
	return this->m_bPanelActive;
}

void MaterialBankPanel::do_mat_list(MaterialQuantityMap& itemList)
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
				ss << pMaterial->get_name();

				ss << " (" << item.second << ")" << std::endl;

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