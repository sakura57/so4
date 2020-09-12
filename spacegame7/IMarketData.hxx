#pragma once

#include "Defs.hxx"

#include "IMaterial.hxx"

interface IMarketData
{
	virtual float get_material_price(IMaterial *, unsigned int const) = 0;
};