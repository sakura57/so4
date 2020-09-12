#pragma once

#include <unordered_map>
#include "IMarketManager.hxx"
#include "IMarketData.hxx"

class CUniverseMarket : public IMarketManager
{
public:
	CUniverseMarket();
	~CUniverseMarket();

	virtual IMarketData* get_market_data(BaseId const);

	void set_market_data(BaseId const, IMarketData * const);

private:
	IMarketData* m_pDefaultMarketData;
	std::unordered_map<BaseId, IMarketData*> m_mMarketDataMap;
};