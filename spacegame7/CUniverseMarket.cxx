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
#include "CUniverseMarket.hxx"
#include "CStaticMarketData.hxx"

CUniverseMarket::CUniverseMarket()
 : m_pDefaultMarketData(new CStaticMarketData)
{

}

CUniverseMarket::~CUniverseMarket()
{
	for(auto i = this->m_mMarketDataMap.begin(); i != this->m_mMarketDataMap.end(); ++i)
	{
		if(i->second != nullptr)
		{
			delete i->second;
		}
	}

	delete this->m_pDefaultMarketData;
}

IMarketData* CUniverseMarket::get_market_data(BaseId const uiBaseId)
{
	if(uiBaseId <= 0)
	{
		return this->m_pDefaultMarketData;
	}

	auto i = this->m_mMarketDataMap.find(uiBaseId);

	if(i != this->m_mMarketDataMap.end())
	{
		return i->second;
	}
	else
	{
		return this->m_pDefaultMarketData;
	}
}

void CUniverseMarket::set_market_data(BaseId const uiBaseId, IMarketData* const pMarketData)
{
	this->m_mMarketDataMap[uiBaseId] = pMarketData;
}