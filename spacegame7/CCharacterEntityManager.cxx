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
#include "CCharacterEntityManager.hxx"
#include "CPlayerCharacterEntity.hxx"
#include "CNonPlayerCharacterEntity.hxx"
#include "SGLib.hxx"
#include "Spells.hxx"
#include "CCharacterEntitiesSaver.hxx"

CCharacterEntityManager::CCharacterEntityManager()
{
	//index 0 is a null entity
	this->m_vCharacterEntities.push_back(nullptr);

	this->create_player_entity();
}

CCharacterEntityManager::~CCharacterEntityManager()
{
	
}

ICharacterEntity *CCharacterEntityManager::get_character_entity(CharacterEntityId const entityId)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(entityId >= this->m_vCharacterEntities.size())
	{
		return nullptr;
	}

	return this->m_vCharacterEntities[entityId];
}

ICharacterEntity *CCharacterEntityManager::get_player_character_entity()
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_vCharacterEntities[this->m_iPlayerEntity];
}

void CCharacterEntityManager::tick(float const flDelta)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	for(ICharacterEntity *pCharEntity : this->m_vCharacterEntities)
	{
		if(pCharEntity == nullptr)
		{
			continue;
		}

		pCharEntity->tick(flDelta);
	}
}

void CCharacterEntityManager::create_player_entity(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	Loadout const *pStartingLoadout = SG::get_game_data_manager()->get_loadout(STARTING_LOADOUT);

	CPlayerCharacterEntity *pPlayerEntity = new CPlayerCharacterEntity(pStartingLoadout, "Player", true);

	IEntityInventory *pInventory = pPlayerEntity->get_inventory();

	pInventory->set_money(STARTING_MONEY);
	pInventory->set_metal(STARTING_METAL);

	//in the case of having exactly the null character
	if(this->m_vCharacterEntities.size() == 1)
	{
		this->m_vCharacterEntities.push_back(pPlayerEntity);
		this->m_iPlayerEntity = 1;
	}
	else
	{
		if(this->m_vCharacterEntities[this->m_iPlayerEntity] != nullptr)
		{
			delete this->m_vCharacterEntities[this->m_iPlayerEntity];
			this->m_vCharacterEntities[this->m_iPlayerEntity] = pPlayerEntity;
		}
	}

	//TESTING CODE
	//pPlayerEntity->learn_spell(SPELL_NAVCOM_VIRUS);
	//pPlayerEntity->learn_spell(SPELL_NANOBOT_STREAM);
	//pPlayerEntity->learn_spell(SPELL_NANOBOT_SWARM);
	//pPlayerEntity->learn_spell(SPELL_MAGNETIC_DRAIN);
	//pPlayerEntity->learn_spell(SPELL_SIGNATURE_SPOOF);
	//pPlayerEntity->learn_spell(SPELL_PERFECT_DEFENSE);
	//pPlayerEntity->learn_spell(SPELL_KINETIC_BARRAGE_WEAK);
	//pPlayerEntity->learn_spell(SPELL_KINETIC_BARRAGE_STRONG);
	//pPlayerEntity->learn_spell(SPELL_LASER_BARRAGE_WEAK);
	//pPlayerEntity->learn_spell(SPELL_LASER_BARRAGE_STRONG);
	//pPlayerEntity->learn_spell(SPELL_MISSILE_SWARM_WEAK);
	//pPlayerEntity->learn_spell(SPELL_MISSILE_SWARM_STRONG);
	pPlayerEntity->set_mapped_spells({0, 0, 0, 0, 0, 0, 0, 0, 0, 0});



	//TODO: please change this to something more reasonable
	this->create_boss_entities();
}

void CCharacterEntityManager::create_boss_entities(void)
{
	CNonPlayerCharacterEntity *pJerichoEntity = new CNonPlayerCharacterEntity(SG::get_game_data_manager()->get_loadout(10), "Jericho", false);
	pJerichoEntity->set_level(4);
	pJerichoEntity->learn_spell(SPELL_NANOBOT_STREAM);
	pJerichoEntity->learn_spell(SPELL_NANOBOT_SWARM);
	pJerichoEntity->learn_spell(SPELL_MISSILE_SWARM_WEAK);
	pJerichoEntity->set_mapped_spells({ SPELL_NANOBOT_STREAM, SPELL_NANOBOT_SWARM, SPELL_MISSILE_SWARM_WEAK, 0, 0, 0, 0, 0, 0, 0 });
	pJerichoEntity->set_mana(pJerichoEntity->get_max_mana());

	this->m_vCharacterEntities.push_back(pJerichoEntity);
}

void CCharacterEntityManager::clear_entities(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	for(ICharacterEntity* pEntity : this->m_vCharacterEntities)
	{
		if(pEntity != nullptr)
		{
			delete pEntity;
		}
	}

	this->m_vCharacterEntities.clear();

	this->m_vCharacterEntities.push_back(nullptr);
}

void CCharacterEntityManager::dump_to_save(std::string const &szSaveFile)
{
	CCharacterEntitiesSaver(this).dump_to_save(szSaveFile);
}

void CCharacterEntityManager::load_from_save(std::string const &szSaveFile)
{
	CCharacterEntitiesSaver(this).load_from_save(szSaveFile);
}