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

interface IStatusEffect
{
	virtual StaticTextureId get_icon_texture(void) const = 0;
	virtual std::string get_name(void) const = 0;
	virtual std::string get_desc(void) const = 0;
	virtual void tick(float const) = 0;
	virtual bool expired(void) = 0;

	virtual float get_hull_damage_multiplier(void) const = 0;
	virtual float get_shield_damage_multiplier(void) const = 0;
	virtual float get_hull_drain_percent(void) const = 0;
	virtual float get_shield_drain_percent(void) const = 0;
	virtual float get_capacitor_drain_percent(void) const = 0;
	virtual float get_engine_thrust_multiplier(void) const = 0;
};