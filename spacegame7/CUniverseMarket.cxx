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