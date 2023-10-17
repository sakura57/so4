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
#include "Util.hxx"

enum class MapObjectShape
{
	SQUARE,
	TRIANGLE,
	CIRCLE
};

struct MapColor
{
	unsigned int r;
	unsigned int g;
	unsigned int b;
};

interface ISectorMapRenderer
{
	virtual void initialize_map(Vector2f const &) = 0;
	virtual void add_map_object(MapObjectShape const, Vector2f const &, MapColor const &) = 0;
	virtual void add_map_zone_circular(Vector2f const &, float const, MapColor const &) = 0;
	virtual void add_map_zone_rectangular(Vector2f const &, Vector2f const &, MapColor const&) = 0;
	virtual void update_player_position(Vector2f const&) = 0;
	virtual void set_map_dimensions(Vector2f const &) = 0;
	virtual void finalize_map(void) = 0;
	virtual void clear_map(void) = 0;
	virtual void draw_map(void) = 0;
	virtual void* get_map_texture_handle(void) = 0;
	virtual void enable_player_icon(bool) = 0;
};