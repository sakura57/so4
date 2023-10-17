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
#include "MaterialType.hxx"
#include "MaterialAttribute.hxx"
#include <tuple>
#include <unordered_map>

typedef std::tuple<float, float> AttributeRange;
typedef unsigned int MaterialClassId;

interface IMaterialClass
{
	virtual std::string get_name(void) = 0;
	virtual MaterialType get_type(void) = 0;
	virtual AttributeRange get_attribute_range(MaterialAttribute const attribute) = 0;
	virtual unsigned int get_generation_weight(void) = 0;
	virtual void construct_randomized_attribute_panel(std::unordered_map<MaterialAttribute, float>&) = 0;

	// -----------------------------------------------------------------------

	static std::string get_material_type_name(MaterialType const);
	static std::string get_material_type_nick(MaterialType const);
	static MaterialType get_material_type_from_nick(std::string const &);

	static std::string get_material_attrib_name(MaterialAttribute const);
	static std::string get_material_attrib_nick(MaterialAttribute const);
	static MaterialAttribute get_material_attrib_from_nick(std::string const&);
};