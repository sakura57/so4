#pragma once

#include <string>
#include "Archetype.hxx"
#include "Loadout.hxx"
#include "EngineProtos.hxx"
#include "Faction.hxx"
#include "Pilot.hxx"

typedef unsigned int CharacterId;

struct Character
{
	CharacterId id;
	std::string name;
	std::string class_type;
	NameCollectionId nameColl;
	ArchId shipArchId;
	FactionId factionId;
	LoadoutId loadoutId;
	PilotId pilotId;
	bool keyCharacter;
	DropTableId dropTableId;
	unsigned long ulExpValue;
	CharacterEntityId entityId;
};