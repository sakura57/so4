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
#include "CMaterialsSaver.hxx"
#include "ini_parser.h"
#include "CGameDataManager.hxx"
#include "SGLib.hxx"
#include "CResourceMaterial.hxx"

#define LOADER_MAX_SECTIONS_BUFFER_SIZE 8192
#define LOADER_MAX_VALUE_BUFFER_SIZE 2048

CMaterialsSaver::CMaterialsSaver(IMatManager* pMatMgr)
{
	this->m_pMatMgr = pMatMgr;
}

void CMaterialsSaver::dump_to_save(std::string const& saveFile)
{
	IMatManager *pMatManager = this->m_pMatMgr;

	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szStringBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];
	char const* szSaveFile = saveFile.c_str();

	bool bFoundHeaderSection = false;
	bool bFirst = true;

	std::string s;
	std::stringstream ss;

	//time to save mat manager data
	{
		MatSerialMap matSerialMap;
		pMatManager->get_materials(matSerialMap);

		std::unordered_map<AstfieldId, IMaterial*> matAssociations;

		pMatManager->get_material_associations(matAssociations);

		ss.clear();
		ss.str("");
		for(auto& p : matAssociations)
		{
			if(!bFirst) {
				ss << " ";
			}

			ss << p.first;
			ss << " ";
			ss << p.second->get_serial_number();

			if(bFirst)
			{
				bFirst = false;
			}

			//vMaterialsToSave.push_back(p.second->get_serial_number());
		}

		s = Conversion::float_to_string(pMatManager->get_time_since_last_update());
		ReadWriteIniKeyValueStringA("MatMgr", "time_since_last_update", s.c_str(), saveFile.c_str(), true);

		ReadWriteIniKeyValueStringA("MatMgr", "assocs", ss.str().c_str(), saveFile.c_str(), true);

		for(auto p : matSerialMap)
		{
			/*if(std::find(vMaterialsToSave.begin(), vMaterialsToSave.end(), p.first) == vMaterialsToSave.end())
			{
				continue;
			}
			*/
			IMaterial* pMaterial = p.second;

			
			unsigned int uiMatClassId = pMaterial->get_material_class();

			std::string szMatSectionName = "material_" + pMaterial->get_serial_number();

			ReadWriteIniKeyValueStringA(szMatSectionName.c_str(), "name", pMaterial->get_name().c_str(), saveFile.c_str(), true);
			ReadWriteIniKeyValueStringA(szMatSectionName.c_str(), "serial", pMaterial->get_serial_number().c_str(), saveFile.c_str(), true);
			s = Conversion::uint_to_string(uiMatClassId);
			ReadWriteIniKeyValueStringA(szMatSectionName.c_str(), "class", s.c_str(), saveFile.c_str(), true);

			std::unordered_map<MaterialAttribute, float> attribSet;

			pMaterial->get_attribute_set(attribSet);

			ss.clear();
			ss.str("");
			for(auto& p : attribSet)
			{
				if(!bFirst) {
					ss << " ";
				}

				ss << IMaterialClass::get_material_attrib_nick(p.first);
				ss << " ";
				ss << p.second;

				if(bFirst)
				{
					bFirst = false;
				}
			}

			ReadWriteIniKeyValueStringA(szMatSectionName.c_str(), "attribs", ss.str().c_str(), saveFile.c_str(), true);
		}
	}
}

void CMaterialsSaver::load_from_save(std::string const& saveFile)
{
	IMatManager* pMatManager = this->m_pMatMgr;

	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szStringBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];
	char const* szSaveFile = saveFile.c_str();

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, szSaveFile, false, szSectionNamesBuffer, &uiSectionNamesLen);

	char* szSection = szSectionNamesBuffer;

	if(strlen(szSection) == 0)
	{
		throw SGException("save file doesn\'t exist, or contains no sections");
	}

	pMatManager->reset();

	unsigned int uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
	strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

	std::stringstream ss;

	while(*szSection)
	{
		if(!strnicmp(szSection, "material_", 9))
		{
			uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(szSaveFile, szSection, "name", "", szStringBuffer, uiStringLen);
			std::string matName(szStringBuffer);

			uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(szSaveFile, szSection, "serial", "", szStringBuffer, uiStringLen);
			std::string matSerial(szStringBuffer);

			unsigned int uiMatClass = CGameDataManager::read_ini_uint(szSaveFile, szSection, "class", "0");

			IMaterial* pMaterial = new CResourceMaterial(matName, matSerial, uiMatClass);


			std::string szAttribNick;
			float flAttribValue;

			uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(szSaveFile, szSection, "attribs", "", szStringBuffer, uiStringLen);
			ss.clear();
			ss.str(szStringBuffer);
			while(ss >> szAttribNick && ss >> flAttribValue)
			{
				MaterialAttribute attrib = IMaterialClass::get_material_attrib_from_nick(szAttribNick);

				pMaterial->set_attribute(attrib, flAttribValue);
			}

			pMatManager->add_material(matSerial, pMaterial);
		}

		szSection += strlen(szSection) + 1;
	}

	{
		std::string szMatSerial;
		unsigned int uiMatQuantity;
		AstfieldId uiAstfieldId;

		float flLastUpdateTime = CGameDataManager::read_ini_float(szSaveFile, "MatMgr", "time_since_last_update", "0");
		pMatManager->set_last_update_time(flLastUpdateTime);

		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
		CGameDataManager::read_ini_string(szSaveFile, "MatMgr", "assocs", "", szStringBuffer, uiStringLen);
		ss.clear();
		ss.str(szStringBuffer);
		while(ss >> uiAstfieldId && ss >> szMatSerial)
		{
			IMaterial* pMaterial = pMatManager->get_material_from_serial(szMatSerial);

			if(pMaterial == nullptr)
			{
				throw SGException("Attempted to create assoc with nonexistant material");
			}

			pMatManager->make_hazy_association(uiAstfieldId, pMaterial);

			pMaterial->increment_reference_count();
		}
	}
}