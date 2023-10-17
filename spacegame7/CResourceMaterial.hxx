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

#include "Util.hxx"
#include "Defs.hxx"
#include "IMaterial.hxx"
#include <unordered_map>

class CResourceMaterial : public IMaterial
{
public:
	CResourceMaterial(std::string const &, std::string const &, MaterialClassId);
	~CResourceMaterial();

	virtual std::string get_name(void);
	virtual std::string get_serial_number(void);
	virtual MaterialClassId get_material_class(void);

	/*
	 * no input validity checks
	 */
	virtual void set_attribute(MaterialAttribute const, float const);

	/*
	 * no input validity checks
	 */
	virtual float get_attribute(MaterialAttribute const);

	virtual void get_attribute_set(std::unordered_map<MaterialAttribute, float> &);

	void copy_attributes(std::unordered_map<MaterialAttribute, float> const&);

	virtual unsigned int get_reference_count(void);
	virtual void increment_reference_count(void);
	virtual void decrement_reference_count(void);

private:
	Spinlock m_mFieldAccess;
	std::string m_sName;
	std::string m_sSerial;
	MaterialClassId m_uiClassId;
	std::unordered_map<MaterialAttribute, float> m_mAttribMap;
	unsigned int m_uiReferences;
};