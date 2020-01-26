#pragma once

#include "Util.hxx"
#include "ICharacterEntity.hxx"
#include "ISaveable.hxx"

interface ICharacterEntityManager : public ISaveable
{
	virtual ICharacterEntity *get_character_entity(CharacterEntityId const) = 0;
	virtual ICharacterEntity *get_player_character_entity(void) = 0;
	virtual void clear_entities(void) = 0;
	virtual void tick(float const) = 0;
	virtual void create_player_entity(void) = 0;
};