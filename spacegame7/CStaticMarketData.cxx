#include "CStaticMarketData.hxx"

CStaticMarketData::CStaticMarketData()
{

}

CStaticMarketData::~CStaticMarketData()
{

}

float CStaticMarketData::get_material_price(IMaterial *pMaterial, unsigned int const uiQuantity)
{
	MaterialClassId uiMaterialClass = pMaterial->get_material_class();

	float flPPU = this->m_mMaterialPPU[uiMaterialClass];
	float flPQM = this->m_mMaterialPQM[uiMaterialClass];

	float flBasePrice = flPPU * uiQuantity;
	float flQualityPercent = pMaterial->get_attribute(MaterialAttribute::INTEGRITY) / 100.0f;
	float flFinalPrice = flBasePrice + flBasePrice * flPQM * flQualityPercent;

	return flFinalPrice;
}

void CStaticMarketData::set_material_ppu(MaterialClassId const uiMaterialClass, float const flPricePerUnit)
{
	this->m_mMaterialPPU[uiMaterialClass] = flPricePerUnit;
}

void CStaticMarketData::set_material_pqm(MaterialClassId const uiMaterialClass, float const flPriceQualityModifier)
{
	this->m_mMaterialPQM[uiMaterialClass] = flPriceQualityModifier;
}