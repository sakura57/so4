#pragma once

#include <string>
#include "Archetype.hxx"
#include "EngineProtos.hxx"
#include "Character.hxx"

typedef unsigned int FactionId;

struct Faction
{
	FactionId id;
	std::string name;
	std::string short_name;
	std::string description;
};