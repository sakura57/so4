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