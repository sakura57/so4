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

#include "IStatusEffect.hxx"
#include "CTransientStatusEffect.hxx"

class CStatusCoolantLeak : public CTransientStatusEffect
{
public:
	CStatusCoolantLeak(float const flDuration)
	{
		this->m_flTimeRemaining = flDuration;
	};

	virtual StaticTextureId get_icon_texture(void) const
	{
		return 11;
	};

	virtual std::string get_name(void) const
	{
		return "Coolant Leak";
	};

	virtual std::string get_desc(void) const
	{
		return "-0.5% HP per second\n-5% engine power";
	};

	virtual float get_hull_drain_percent(void) const
	{
		return 0.005f;
	};

	virtual float get_engine_thrust_multiplier(void) const
	{
		return 0.95f;
	};
};