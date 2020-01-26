#pragma once

#include <utility>
#include <unordered_map>
#include "Defs.hxx"
#include "Archetype.hxx"
#include "IEntityInventory.hxx"

typedef unsigned int DropTableId;
//typedef std::unordered_map<ArchId, unsigned int> Drops;

interface ILootTableManager
{
	virtual void roll_drops(DropTableId const, QuantityMap &, unsigned int &) = 0;
	virtual void create_drop_table(DropTableId const) = 0;
	virtual void add_drop(DropTableId const, ArchId const, float const) = 0;
	virtual void set_metal_chance(DropTableId const, unsigned int const, unsigned int const) = 0;
};