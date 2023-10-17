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

interface IRadarVisible
{
	enum
	{
		InstanceFlag = 1 << 13
	};

	enum class RadarShape
	{
		Circle,
		Square
	};

	virtual RadarShape get_radar_shape(void) = 0;

	virtual std::string get_radar_name(void) = 0;

	virtual std::string get_radar_class(void) = 0;
};