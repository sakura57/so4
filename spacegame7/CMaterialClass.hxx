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

#include "IMaterialClass.hxx"
#include <unordered_map>
#include <tuple>

class CMaterialClass : public IMaterialClass
{
public:
	CMaterialClass(std::string const &, MaterialType const, unsigned int const);
	~CMaterialClass();

	virtual std::string get_name(void);
	virtual MaterialType get_type(void);
	virtual AttributeRange get_attribute_range(MaterialAttribute const);
	virtual unsigned int get_generation_weight(void);
	virtual void construct_randomized_attribute_panel(std::unordered_map<MaterialAttribute, float> &attribMap);

	void set_attribute_range(MaterialAttribute const, AttributeRange const&);

private:
	std::string m_sName;
	MaterialType m_matType;
	unsigned int m_uiWeight;
	
	std::unordered_map<MaterialAttribute, AttributeRange> m_mAttribRanges;
};