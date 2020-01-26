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
	virtual void add_map_zone_rectangular(Vector2f const &, Vector2f const &) = 0;
	virtual void finalize_map(void) = 0;
	virtual void clear_map(void) = 0;
};