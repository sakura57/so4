#pragma once

#include "Defs.hxx"
#include "EngineProtos.hxx"
#include "IWorldInstance.hxx"
#include <vector>

#define INVALID_SPELL 0
typedef unsigned int SpellId;
typedef std::vector<SpellId> SpellSet;

interface ISpell
{
	virtual std::string get_name(void) = 0;
	virtual std::string get_desc(void) = 0;
	virtual StaticTextureId get_icon_texture(void) = 0;
	virtual bool cast(InstanceId const, InstanceId const) = 0;
	virtual float get_cooldown(void) = 0;
	virtual float get_mana_cost(void) = 0;
};