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
#include "CCharacterEntitiesSaver.hxx"
#include "ini_parser.h"
#include "CGameDataManager.hxx"
#include "SGLib.hxx"

#define LOADER_MAX_SECTIONS_BUFFER_SIZE 8192
#define LOADER_MAX_VALUE_BUFFER_SIZE 2048

CCharacterEntitiesSaver::CCharacterEntitiesSaver(ICharacterEntityManager *pCEM)
{
	this->m_pCEM = pCEM;
}

void CCharacterEntitiesSaver::dump_to_save(std::string const&saveFile)
{
	ICharacterEntityManager* pCEM = this->m_pCEM;

	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szStringBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];
	char const* szSaveFile = saveFile.c_str();

	bool bFoundHeaderSection = false;
	bool bFirst = true;

	std::string s;
	std::stringstream ss;

	ICharacterEntity* pEntity = pCEM->get_player_character_entity();

	//TODO: don't hardcode player entity ID 1
	char szSection[] = "entity_1";

	if(pEntity != nullptr)
	{
		IEntityInventory* pInventory = pEntity->get_inventory();

		//TODO: don't hardcode player entity ID 1
		s = Conversion::uint_to_string(1U);
		ReadWriteIniKeyValueStringA(szSection, "id", const_cast<char*>(s.c_str()), saveFile.c_str(), true);

		ReadWriteIniKeyValueStringA(szSection, "name", pEntity->get_name().c_str(), saveFile.c_str(), true);

		CStatBlock* pStatBlock = pEntity->get_stat_block();
		std::stringstream ss(szStringBuffer);
		pStatBlock->format_stat_data(s);
		ReadWriteIniKeyValueStringA(szSection, "stats", const_cast<char*>(s.c_str()), saveFile.c_str(), true);

		s = Conversion::uint_to_string((unsigned int)pEntity->get_level());
		ReadWriteIniKeyValueStringA(szSection, "level", const_cast<char*>(s.c_str()), saveFile.c_str(), true);

		s = Conversion::uint_to_string((unsigned int)pEntity->get_current_exp());
		ReadWriteIniKeyValueStringA(szSection, "exp", const_cast<char*>(s.c_str()), saveFile.c_str(), true);

		s = Conversion::uint_to_string((unsigned int)pEntity->get_ship());
		ReadWriteIniKeyValueStringA(szSection, "ship", const_cast<char*>(s.c_str()), saveFile.c_str(), true);

		s = Conversion::uint_to_string((unsigned int)pInventory->get_money());
		ReadWriteIniKeyValueStringA(szSection, "money", const_cast<char*>(s.c_str()), saveFile.c_str(), true);

		s = Conversion::uint_to_string((unsigned int)pInventory->get_metal());
		ReadWriteIniKeyValueStringA(szSection, "metal", const_cast<char*>(s.c_str()), saveFile.c_str(), true);

		s = Conversion::uint_to_string((unsigned int)pEntity->get_skill_points_available());
		ReadWriteIniKeyValueStringA(szSection, "free_skill_points", const_cast<char*>(s.c_str()), saveFile.c_str(), true);

		//Write the skills
		{
			ss.clear();
			ss.str("");
			std::vector<SkillId> skills;
			pEntity->get_skills(skills);

			for(SkillId const skill : skills)
			{
				if(!bFirst) {
					ss << " ";
				}

				ss << skill;

				if(bFirst)
				{
					bFirst = false;
				}
			}

			bFirst = true;
		}

		s = ss.str();
		ReadWriteIniKeyValueStringA(szSection, "known_skills", const_cast<char*>(s.c_str()), saveFile.c_str(), true);

		//Write the abilities
		{
			ss.clear();
			ss.str("");
			std::vector<SpellId> spells = pEntity->get_known_spells();

			for(SpellId const spell : spells)
			{
				if(!bFirst) {
					ss << " ";
				}

				ss << spell;

				if(bFirst)
				{
					bFirst = false;
				}
			}

			bFirst = true;
		}

		s = ss.str();
		ReadWriteIniKeyValueStringA(szSection, "known_abilities", const_cast<char*>(s.c_str()), saveFile.c_str(), true);

		//Write the mapped abilities
		{
			ss.clear();
			ss.str("");
			std::vector<SpellId> spells = pEntity->get_mapped_spells();

			for(SpellId const spell : spells)
			{
				if(!bFirst) {
					ss << " ";
				}

				ss << spell;

				if(bFirst)
				{
					bFirst = false;
				}
			}

			bFirst = true;
		}

		s = ss.str();
		ReadWriteIniKeyValueStringA(szSection, "mapped_abilities", const_cast<char*>(s.c_str()), saveFile.c_str(), true);

		//Write the inventory
		{
			ss.clear();
			ss.str("");
			ItemList itemList;

			pInventory->get_items_in_inventory(itemList);

			for(auto& item : itemList)
			{
				if(!bFirst) {
					ss << " ";
				}

				ss << item.first;
				ss << " ";
				ss << item.second;

				if(bFirst)
				{
					bFirst = false;
				}
			}

			bFirst = true;
		}

		s = ss.str();
		ReadWriteIniKeyValueStringA(szSection, "inventory", const_cast<char*>(s.c_str()), saveFile.c_str(), true);

		//Write the loadout
		{
			ss.clear();
			ss.str("");
			Loadout loadout;

			loadout = pEntity->get_loadout();

			for(auto& item : loadout)
			{
				if(!bFirst) {
					ss << " ";
				}

				ss << item;

				if(bFirst)
				{
					bFirst = false;
				}
			}

			bFirst = true;
		}

		s = ss.str();
		ReadWriteIniKeyValueStringA(szSection, "loadout", const_cast<char*>(s.c_str()), saveFile.c_str(), true);

		//Write the material bank
		{
			ss.clear();
			ss.str("");
			MaterialQuantityMap matQuantMap;

			pEntity->get_material_bank()->get_material_quantity_map(matQuantMap);

			for(auto& p : matQuantMap)
			{
				if(!bFirst) {
					ss << " ";
				}

				ss << p.first;
				ss << " ";
				ss << p.second;

				if(bFirst)
				{
					bFirst = false;
				}
			}

			bFirst = true;
		}

		s = ss.str();
		ReadWriteIniKeyValueStringA(szSection, "mats", const_cast<char*>(s.c_str()), saveFile.c_str(), true);
	}
}

void CCharacterEntitiesSaver::load_from_save(std::string const&saveFile)
{
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

	ICharacterEntityManager* pCEM = this->m_pCEM;

	pCEM->clear_entities();
	pCEM->create_player_entity();

	while(*szSection)
	{
		unsigned int uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		if(!strnicmp(szSection, "entity_", 7))
		{
			CharacterEntityId entityId = CGameDataManager::read_ini_uint(szSaveFile, szSection, "id", "0");

			if(entityId != 0)
			{
				ICharacterEntity* pEntity = pCEM->get_character_entity(entityId);
				IEntityInventory* pInventory = pEntity->get_inventory();

				uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
				strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
				CGameDataManager::read_ini_string(szSaveFile, szSection, "name", "", szStringBuffer, uiStringLen);
				std::string entityName(szStringBuffer);
				pEntity->set_name(entityName);

				uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
				strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
				CGameDataManager::read_ini_string(szSaveFile, szSection, "stats", "", szStringBuffer, uiStringLen);
				std::string statName;
				float flValue;

				CStatBlock statBlock;

				std::stringstream ss(szStringBuffer);
				while(ss >> statName, ss >> flValue)
				{
					//std::cout << "\tGrants " << flValue << " to " << statName << std::endl;
					statBlock.set_stat(CStatBlock::get_stat_from_name(statName), flValue);
				}

				pEntity->set_stats(statBlock);

				int iLevel = (int)CGameDataManager::read_ini_uint(szSaveFile, szSection, "level", "1");
				pEntity->set_level(iLevel);
				pEntity->set_mana(pEntity->get_max_mana());

				unsigned long ulExp = (unsigned long)CGameDataManager::read_ini_uint(szSaveFile, szSection, "exp", "0");
				pEntity->gain_exp(ulExp);

				unsigned int uiShip = CGameDataManager::read_ini_uint(szSaveFile, szSection, "ship", "1");
				pEntity->set_ship(uiShip);

				int uiMoney = (int)CGameDataManager::read_ini_uint(szSaveFile, szSection, "money", "0");
				pInventory->set_money(uiMoney);

				int uiMetal = (int)CGameDataManager::read_ini_uint(szSaveFile, szSection, "metal", "0");
				pInventory->set_metal(uiMetal);

				unsigned int uiFreeSkillPoints = CGameDataManager::read_ini_uint(szSaveFile, szSection, "free_skill_points", "0");
				pEntity->set_free_skill_points(uiFreeSkillPoints);

				SpellSet mappedSpells;

				SpellId spell;
				SkillId skill;

				pEntity->clear_skills();

				uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
				strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
				CGameDataManager::read_ini_string(szSaveFile, szSection, "known_skills", "", szStringBuffer, uiStringLen);
				ss.clear();
				ss.str(szStringBuffer);
				while(ss >> skill)
				{
					pEntity->give_skill(skill);
				}

				pEntity->reset_spells();

				uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
				strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
				CGameDataManager::read_ini_string(szSaveFile, szSection, "known_abilities", "", szStringBuffer, uiStringLen);
				ss.clear();
				ss.str(szStringBuffer);
				while(ss >> spell)
				{
					pEntity->learn_spell(spell);
				}

				uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
				strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
				CGameDataManager::read_ini_string(szSaveFile, szSection, "mapped_abilities", "", szStringBuffer, uiStringLen);
				ss.clear();
				ss.str(szStringBuffer);
				while(ss >> spell)
				{
					mappedSpells.push_back(spell);
				}

				pEntity->set_mapped_spells(mappedSpells);

				ArchId equipId;
				QuantityMap items;
				unsigned int quantity;
				pInventory->clear_items();

				uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
				strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
				CGameDataManager::read_ini_string(szSaveFile, szSection, "inventory", "", szStringBuffer, uiStringLen);
				ss.clear();
				ss.str(szStringBuffer);
				while(ss >> equipId, ss >> quantity)
				{
					items[equipId] += quantity;
				}

				pInventory->add_items(items);

				Loadout loadout;

				uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
				strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
				CGameDataManager::read_ini_string(szSaveFile, szSection, "loadout", "", szStringBuffer, uiStringLen);
				ss.clear();
				ss.str(szStringBuffer);
				while(ss >> equipId)
				{
					loadout.push_back(equipId);
				}

				pEntity->set_loadout(loadout);

				std::string szMatSerial;
				unsigned int uiMatQuantity;
				IMaterialBank* pMatBank = pEntity->get_material_bank();

				uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
				strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
				CGameDataManager::read_ini_string(szSaveFile, szSection, "mats", "", szStringBuffer, uiStringLen);
				ss.clear();
				ss.str(szStringBuffer);
				while(ss >> szMatSerial && ss >> uiMatQuantity)
				{
					if(uiMatQuantity == 0)
					{
						continue;
					}

					pMatBank->set_material_quantity(szMatSerial, uiMatQuantity);

					IMaterial* pMaterial = SG::get_material_manager()->get_material_from_serial(szMatSerial);

					if(pMaterial == nullptr)
					{
						throw SGException("Need to increment ref count for a nonexistant material");
					}
					else
					{
						pMaterial->increment_reference_count();
					}
				}
			}
		}

		szSection += strlen(szSection) + 1;
	}
}