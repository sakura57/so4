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
#pragma once

#include <vector>
#include <sstream>
#include <mutex>
#include "ICharacterEntityManager.hxx"

#define STARTING_LOADOUT 1U
#define STARTING_MONEY 1000
#define STARTING_METAL 0

class CCharacterEntityManager : public ICharacterEntityManager
{
public:
	CCharacterEntityManager();
	~CCharacterEntityManager();

	//members inherited from ISaveable
	virtual void dump_to_save(std::string const&) final;
	virtual void load_from_save(std::string const&) final;

	virtual ICharacterEntity *get_character_entity(CharacterEntityId const) final;
	virtual ICharacterEntity *get_player_character_entity(void) final;
	virtual void tick(float const) final;

	virtual void clear_entities(void) final;

	virtual void create_player_entity(void);

private:
	void create_boss_entities(void);

	std::mutex m_mFieldAccess;

	std::vector<ICharacterEntity*> m_vCharacterEntities;
	CharacterEntityId m_iPlayerEntity;
};