#pragma once

#include "Defs.hxx"
#include "IUniverse.hxx"
#include "IMarketData.hxx"

interface IMarketManager
{
	virtual IMarketData *get_market_data(BaseId const) = 0;
};