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