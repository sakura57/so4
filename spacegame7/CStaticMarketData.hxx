#pragma once

#include <unordered_map>
#include "IMaterial.hxx"
#include "IMaterialClass.hxx"
#include "IMarketData.hxx"

class CStaticMarketData : public IMarketData
{
public:
	CStaticMarketData();
	~CStaticMarketData();

	virtual float get_material_price(IMaterial *, unsigned int const);

	void set_material_ppu(MaterialClassId const, float const);
	void set_material_pqm(MaterialClassId const, float const);

private:
	std::unordered_map<MaterialClassId, float> m_mMaterialPPU;
	std::unordered_map<MaterialClassId, float> m_mMaterialPQM;
};