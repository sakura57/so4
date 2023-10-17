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
#include "IMaterialClass.hxx"
#include <unordered_map>

typedef std::unordered_map<std::string, unsigned int> MaterialQuantityMap;

interface IMaterialBank
{
	virtual void get_material_quantity_map(MaterialQuantityMap &) = 0;
	virtual void set_material_quantity_map(MaterialQuantityMap const &) = 0;
	virtual unsigned int add_material(std::string const&, unsigned int const) = 0;
	virtual unsigned int get_material_quantity(std::string const &) = 0;
	virtual void set_material_quantity(std::string const&, unsigned int const) = 0;
};