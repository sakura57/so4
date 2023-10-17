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