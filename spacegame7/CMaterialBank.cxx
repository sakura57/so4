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
#include "CMaterialBank.hxx"
#include "Util.hxx"
#include "SGLib.hxx"

CMaterialBank::CMaterialBank()
{

}

CMaterialBank::~CMaterialBank()
{

}

void CMaterialBank::get_material_quantity_map(MaterialQuantityMap &matQuantityMap)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	matQuantityMap = this->m_mMatQuantityMap;
}

void CMaterialBank::set_material_quantity_map(MaterialQuantityMap const&matQuantityMap)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_mMatQuantityMap = matQuantityMap;
}

unsigned int CMaterialBank::add_material(std::string const &szMaterialSerial, unsigned int const uiQuantity)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	IMaterial* pMaterial = SG::get_material_manager()->get_material_from_serial(szMaterialSerial);

	if(pMaterial == nullptr)
	{
		return 0;
	}

	auto i = this->m_mMatQuantityMap.find(szMaterialSerial);

	if(i != this->m_mMatQuantityMap.end())
	{
		this->m_mMatQuantityMap[szMaterialSerial] += uiQuantity;
	}
	else
	{
		this->m_mMatQuantityMap[szMaterialSerial] = uiQuantity;

		pMaterial->increment_reference_count();
	}
	
	return this->m_mMatQuantityMap[szMaterialSerial];
}

unsigned int CMaterialBank::get_material_quantity(std::string const& szMaterialSerial)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_mMatQuantityMap[szMaterialSerial];
}

void CMaterialBank::set_material_quantity(std::string const& szMaterialSerial, unsigned int const uiQuantity)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_mMatQuantityMap[szMaterialSerial] = uiQuantity;

	if(uiQuantity == 0)
	{
		IMaterial* pMaterial = SG::get_material_manager()->get_material_from_serial(szMaterialSerial);

		if(pMaterial == nullptr)
		{
			throw SGException("Need to decrement ref count for a seemingly nonexistant material");
		}
		else
		{
			pMaterial->decrement_reference_count();
		}
	}
}