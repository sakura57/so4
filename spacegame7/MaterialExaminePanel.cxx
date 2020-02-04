#include "MaterialExaminePanel.hxx"
#include "LoadoutPanel.hxx"
#include "SGLib.hxx"
#include <sstream>

int MaterialExaminePanel::m_iPanelInstances = 0;

void MaterialExaminePanel::render_panel(float const flDelta)
{
	if(this->m_szMaterialSerial == "")
	{
		return;
	}

	ImGui::Begin("Examine Material");

	ImGui::Text(this->m_szIdText.c_str());

	ImGui::Separator();

	ImGui::Text(this->m_szMatAttribsText.c_str());

	ImGui::Separator();

	if(ImGui::Button("Done"))
	{
		this->m_bPanelActive = false;
	}

	ImGui::End();
}

bool MaterialExaminePanel::panel_active(void)
{
	return this->m_bPanelActive;
}

void MaterialExaminePanel::build_attribs_text_block(void)
{
	IMatManager* pMatManager = SG::get_material_manager();
	IMaterial* pMaterial = pMatManager->get_material_from_serial(this->m_szMaterialSerial);
	IMaterialClass* pMatClass = pMatManager->get_material_class_from_id(pMaterial->get_material_class());

	std::unordered_map<MaterialAttribute, float> matAttribs;

	pMaterial->get_attribute_set(matAttribs);

	std::stringstream ss("");

	ss.precision(1);

	ss << std::fixed;

	for(auto i : matAttribs)
	{
		std::string szAttribName = IMaterialClass::get_material_attrib_name(i.first);
		ss << szAttribName;
		ss << ": ";
		ss << i.second;

		ss << "\n";
	}

	this->m_szMatAttribsText = ss.str();
}

void MaterialExaminePanel::build_id_text_block(void)
{
	IMatManager* pMatManager = SG::get_material_manager();
	IMaterial* pMaterial = pMatManager->get_material_from_serial(this->m_szMaterialSerial);
	IMaterialClass* pMatClass = pMatManager->get_material_class_from_id(pMaterial->get_material_class());

	std::string szMatTypeName = IMaterialClass::get_material_type_name(pMatClass->get_type());

	std::stringstream ss("");

	ss << "Name: " << pMaterial->get_name() << "\n";
	ss << "Serial: " << pMaterial->get_serial_number() << "\n";
	ss << "Type: " << szMatTypeName << "\n";
	ss << "Class: " << pMatClass->get_name();

	this->m_szIdText = ss.str();
}