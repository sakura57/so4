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