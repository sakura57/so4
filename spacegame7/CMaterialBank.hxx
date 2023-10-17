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

#include "Defs.hxx"
#include <mutex>
#include "IMaterialBank.hxx"

class CMaterialBank : public IMaterialBank
{
public:
	CMaterialBank();
	~CMaterialBank();

	/*
	 * UNSAFE with regard to mat reference count, do NOT use unless you
	 * have otherwise controlled ref counts for mats
	 */
	virtual void get_material_quantity_map(MaterialQuantityMap &);

	/*
	 * UNSAFE with regard to mat reference count, do NOT use unless you
	 * have otherwise controlled ref counts for mats
	 */
	virtual void set_material_quantity_map(MaterialQuantityMap const &);

	/*
	 * SAFE with regard to mat reference count
	 */
	virtual unsigned int add_material(std::string const &, unsigned int const);
	virtual unsigned int get_material_quantity(std::string const &);

	/*
	 * SAFE with regard to mat reference count
	 */
	virtual void set_material_quantity(std::string const &, unsigned int const);

private:
	std::mutex m_mFieldAccess;
	MaterialQuantityMap m_mMatQuantityMap;
};