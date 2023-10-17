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