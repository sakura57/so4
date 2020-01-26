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