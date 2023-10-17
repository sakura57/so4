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
#include "CMatManager.hxx"
#include "CResourceMaterial.hxx"
#include <vector>
#include "SGLib.hxx"
#include "CMaterialsSaver.hxx"

#define SERIAL_NUMBER_LENGTH 8
#define MIN_MAT_NAME_LENGTH 4
#define MAX_MAT_NAME_LENGTH 5

#define MAT_GENERATION_TICK_LENGTH 3600.0f //one hour

#define MIN_MAT_POOL_SIZE 30

CMatManager::CMatManager()
{
	this->m_flLastUpdate = 0.0f;
}


CMatManager::~CMatManager()
{
	for(auto p : this->m_mMatSerialMap)
	{
		delete p.second;
	}
}

void CMatManager::dump_to_save(std::string const &saveFile)
{
	CMaterialsSaver(this).dump_to_save(saveFile);
}

void CMatManager::load_from_save(std::string const & saveFile)
{
	CMaterialsSaver(this).load_from_save(saveFile);
}


void CMatManager::reset(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vMaterialPool.clear();
	this->m_mAstfieldMaterialMapping.clear();

	for(auto p : this->m_mMatSerialMap)
	{
		delete p.second;
	}

	this->m_mMatSerialMap.clear();

	this->m_flLastUpdate = 0.0f;
}

void CMatManager::force_generation_tick(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flLastUpdate = SG::get_game_clock()->get_absolute_delta();

	this->regenerate_material_pool();
}

void CMatManager::do_possible_generation_tick(float const flDelta)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(flDelta - this->m_flLastUpdate > MAT_GENERATION_TICK_LENGTH)
	{
		this->m_flLastUpdate = flDelta;

		this->regenerate_material_pool();
	}
}

void CMatManager::add_material_category(MaterialClassId const uiMatClassId,  IMaterialClass* pMatClass)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(uiMatClassId >= this->m_vMaterialClasses.size())
	{
		this->m_vMaterialClasses.resize(uiMatClassId + 1);
	}

	this->m_vMaterialClasses[uiMatClassId] = pMatClass;
}

std::string CMatManager::generate_serial_number()
{
	std::vector<char> static const valid_chars =
	{
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
	};

	std::string s;

	std::uniform_int_distribution<size_t> charDist(0U, valid_chars.size() - 1);

	for (size_t i = 0; i < SERIAL_NUMBER_LENGTH; ++i)
	{
		size_t chosenChar = charDist(SG::get_random());

		s.append(1, valid_chars[chosenChar]);
	}

	return s;
}

std::string CMatManager::generate_name()
{
	std::vector<std::string> static const vowels_single =
	{
		"a", "e", "i", "o", "u"
	};

	std::vector<std::string> static const consonants_single =
	{
		"b", "d", "f", "g", "h", "j", "k", "l", "m", "n", "p", "q", "r", "s", "t", "v", "w", "y", "z"
	};

	std::vector<std::string> static const consonants_single_word_initial =
	{
		"B", "D", "F", "G", "H", "J", "K", "L", "M", "N", "P", "R", "S", "T", "V", "W", "Y"
	};

	std::vector<std::string> static const vowels_diphthong =
	{
		"ai", "ea", "oa", "ou"
	};

	std::vector<std::string> static const consonants_diphthong =
	{
		"br", "cr", "dr", "gr", "pr", "tr", "vr", "wr", "fr", "ch", "th", "fl", "gl", "kl"
	};

	std::vector<std::string> static const suffixes =
	{
		"ite"
	};

	std::string s;

	size_t len;
	{
		std::uniform_int_distribution<size_t> lenDist(MIN_MAT_NAME_LENGTH, MAX_MAT_NAME_LENGTH);

		len = lenDist(SG::get_random());
	}

	for(size_t i = 0; i < len; ++i)
	{
		if(i == 0)
		{
			std::uniform_int_distribution<size_t> selectDist(0U, consonants_single_word_initial.size() - 1);

			size_t select = selectDist(SG::get_random());

			s.append(consonants_single_word_initial[select]);
		}
		else if(i % 2 == 0) //even index
		{
			std::uniform_int_distribution<size_t> determineSingleOrDiphthongDist(0U, 20U);

			size_t singleOrDiphthongWeight;

			if(i < len - 1)
			{
				singleOrDiphthongWeight = determineSingleOrDiphthongDist(SG::get_random());
			}
			else
			{
				singleOrDiphthongWeight = 20U;
			}

			if(singleOrDiphthongWeight < 5U) //diphthong
			{
				std::uniform_int_distribution<size_t> selectDist(0U, consonants_diphthong.size() - 1);

				size_t select = selectDist(SG::get_random());

				s.append(consonants_diphthong[select]);
			}
			else
			{
				std::uniform_int_distribution<size_t> selectDist(0U, consonants_single.size() - 1);

				size_t select = selectDist(SG::get_random());

				s.append(consonants_single[select]);
			}
		}
		else //odd index
		{
			std::uniform_int_distribution<size_t> determineSingleOrDiphthongDist(0U, 20U);

			size_t singleOrDiphthongWeight;

			if(i < len - 1)
			{
				singleOrDiphthongWeight = determineSingleOrDiphthongDist(SG::get_random());
			}
			else
			{
				singleOrDiphthongWeight = 20U;
			}

			if(singleOrDiphthongWeight < 5U) //diphthong
			{
				std::uniform_int_distribution<size_t> selectDist(0U, vowels_diphthong.size() - 1);

				size_t select = selectDist(SG::get_random());

				s.append(vowels_diphthong[select]);
			}
			else
			{
				std::uniform_int_distribution<size_t> selectDist(0U, vowels_single.size() - 1);

				size_t select = selectDist(SG::get_random());

				s.append(vowels_single[select]);
			}
		}
	}

	//determine whether to add suffix
	if(s.size() < 8)
	{
		std::uniform_int_distribution<size_t> determineAddSuffixDist(0U, 40U);

		size_t addSuffixWeight = determineAddSuffixDist(SG::get_random());

		if(addSuffixWeight < 5U)
		{
			std::uniform_int_distribution<size_t> selectDist(0U, suffixes.size() - 1);

			size_t select = selectDist(SG::get_random());

			s.append(suffixes[select]);
		}
	}

	return s;
}

/*
void CMatManager::empty_material_pools()
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->clear_material_pool();
	this->cleanup_materials();

	this->m_flLastUpdate = -100000.0f;
}
*/

void CMatManager::regenerate_material_pool()
{
	this->clear_material_pool();

	//also clear the astfield mat mapping.
	//astfields will be assigned new materials.
	this->m_mAstfieldMaterialMapping.clear();

	this->cleanup_materials();
	this->generate_materials(MIN_MAT_POOL_SIZE);
}

void CMatManager::clear_material_pool()
{
	//we need to iterate through each mat in the material pool.
	//decrement reference count by one since we're removing it from
	//the pool, if the player doesn't have any then it'll be
	//removed permanently in the cleanup phase

	for(auto p : this->m_vMaterialPool)
	{
		for(IMaterial* pMaterial : p.second)
		{
			pMaterial->decrement_reference_count();
		}
	}

	this->m_vMaterialPool.clear();
}

void CMatManager::cleanup_materials()
{
	MatSerialMap::iterator i = this->m_mMatSerialMap.begin();

	while(i != this->m_mMatSerialMap.end())
	{
		IMaterial* pMat = i->second;

		if(pMat->get_reference_count() == 0)
		{
			delete pMat;

			this->m_mMatSerialMap.erase(i++);
		}
		else
		{
			++i;
		}
	}
}

void CMatManager::build_material_pools(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->clear_material_pool();
	this->cleanup_materials();
	this->generate_materials(MIN_MAT_POOL_SIZE);
}

float CMatManager::get_time_since_last_update(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);
	
	return SG::get_game_clock()->get_absolute_delta() - this->m_flLastUpdate;
}

void CMatManager::set_last_update_time(float const flTimeSinceLastUpdate)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flLastUpdate = SG::get_game_clock()->get_absolute_delta() - flTimeSinceLastUpdate;
}

void CMatManager::generate_materials(int const n)
{
	for(size_t i = 0; i < this->m_vMaterialClasses.size(); ++i)
	{
		IMaterialClass* pMatClass = this->m_vMaterialClasses[i];

		if(pMatClass)
		{
			std::deque<IMaterial *> &matPool = this->m_vMaterialPool[pMatClass->get_type()];

			for(size_t j = 0; j < pMatClass->get_generation_weight(); ++j)
			{
				std::string sMatName = generate_name();
				std::string sSerial = generate_serial_number();

				CResourceMaterial* pMat = new CResourceMaterial(sMatName, sSerial, i);

				std::unordered_map<MaterialAttribute, float> matAttributes;

				pMatClass->construct_randomized_attribute_panel(matAttributes);

				pMat->copy_attributes(matAttributes);

				pMat->increment_reference_count();

				matPool.push_back(pMat);

				this->m_mMatSerialMap[sSerial] = pMat;
			}

			std::shuffle(matPool.begin(), matPool.end(), SG::get_random());
		}
	}
}

IMaterial * CMatManager::pull_random_material_from_pool(MaterialType const matType)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	//If the pool is empty, regenerate it.
	if(this->m_vMaterialPool[matType].size() == 0)
	{
		this->regenerate_material_pool();
	}

	IMaterial* pMaterial = this->m_vMaterialPool[matType].front();

	pMaterial->decrement_reference_count();

	this->m_vMaterialPool[matType].pop_front();

	return pMaterial;
}

IMaterial* CMatManager::get_material_from_serial(std::string const& szSerial)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	auto i = this->m_mMatSerialMap.find(szSerial);

	if(i == this->m_mMatSerialMap.end())
	{
		return nullptr;
	}
	else
	{
		return i->second;
	}
}

IMaterial* CMatManager::pull_random_material_from_pool_hazy_associative(AstfieldId const astfieldId, MaterialType const matType)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	auto i = this->m_mAstfieldMaterialMapping.find(astfieldId);

	IMaterial* pMaterial = nullptr;

	if(i == this->m_mAstfieldMaterialMapping.end())
	{
		//If the pool is empty, regenerate it.
		if(this->m_vMaterialPool[matType].size() == 0)
		{
			this->regenerate_material_pool();
		}

		pMaterial = this->m_vMaterialPool[matType].front();

		pMaterial->decrement_reference_count();

		this->m_vMaterialPool[matType].pop_front();

		this->m_mAstfieldMaterialMapping[astfieldId] = pMaterial;
	}
	else
	{
		pMaterial = i->second;
	}
	

	return pMaterial;
}

IMaterialClass* CMatManager::get_material_class_from_id(MaterialClassId const matClassId)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(matClassId >= this->m_vMaterialClasses.size())
	{
		throw SGException("Requested invalid material class ID");
	}

	return this->m_vMaterialClasses[matClassId];
}

void CMatManager::add_material(std::string const &szMaterialSerial, IMaterial *pMaterial)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_mMatSerialMap[szMaterialSerial] = pMaterial;
}

void CMatManager::make_hazy_association(AstfieldId const astfieldId, IMaterial *pMaterial)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_mAstfieldMaterialMapping[astfieldId] = pMaterial;
}

void CMatManager::get_materials(MatSerialMap &matSerialMap)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	matSerialMap = this->m_mMatSerialMap;
}

void CMatManager::get_material_associations(std::unordered_map<AstfieldId, IMaterial*> &matAssociations)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	matAssociations = this->m_mAstfieldMaterialMapping;
}

IMaterial *CMatManager::get_material_association(AstfieldId const uiAstfieldId)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_mAstfieldMaterialMapping[uiAstfieldId];
}

/*
void CMatManager::get_material_pools(std::unordered_map<MaterialType, std::deque<IMaterial*>> &matPools)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	matPools = this->m_vMaterialPool;
}
*/

/*
void CMatManager::add_material_to_pool(IMaterial *pMaterial)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	MaterialClassId matClassId = pMaterial->get_material_class();

	if(matClassId >= this->m_vMaterialClasses.size())
	{
		IMaterialClass* pMatClass = this->m_vMaterialClasses[matClassId];

		if(pMatClass)
		{
			this->m_vMaterialPool[pMatClass->get_type()].push_back(pMaterial);

			pMaterial->increment_reference_count();
		}
		else
		{
			throw SGException("Attempted to add mat to pool with null mat class");
		}
	}
	else
	{
		throw SGException("Attempted to add mat to pool with invalid mat class ID");
	}
}
*/

/*
void CMatManager::shuffle_material_pools(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	for(auto &p : this->m_vMaterialPool)
	{
		std::shuffle(p.second.begin(), p.second.end(), SG::get_random());
	}
}
*/