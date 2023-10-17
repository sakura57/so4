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