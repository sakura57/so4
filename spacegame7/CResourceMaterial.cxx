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
#include "CResourceMaterial.hxx"

CResourceMaterial::CResourceMaterial(std::string const& sName, std::string const& sSerial, MaterialClassId uiClassId)
	: m_sName(sName), m_sSerial(sSerial), m_uiClassId(uiClassId)
{
	this->m_uiReferences = 0U;
}

CResourceMaterial::~CResourceMaterial()
{

}

void CResourceMaterial::set_attribute(MaterialAttribute const attrib, float const flValue)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_mAttribMap[attrib] = flValue;
}

float CResourceMaterial::get_attribute(MaterialAttribute const attrib)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_mAttribMap[attrib];
}

std::string CResourceMaterial::get_name(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_sName;
}

std::string CResourceMaterial::get_serial_number(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_sSerial;
}

MaterialClassId CResourceMaterial::get_material_class(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_uiClassId;
}

void CResourceMaterial::copy_attributes(std::unordered_map<MaterialAttribute, float> const &mAttribMap)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_mAttribMap = mAttribMap;
}

unsigned int CResourceMaterial::get_reference_count()
{
	SCOPE_LOCK(this->m_mFieldAccess);
	
	return this->m_uiReferences;
}

void CResourceMaterial::increment_reference_count()
{
	SCOPE_LOCK(this->m_mFieldAccess);

	++this->m_uiReferences;
}

void CResourceMaterial::decrement_reference_count()
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(this->m_uiReferences == 0)
	{
		throw SGException("Attempted to decrement CResourceMaterial ref count while rc = 0");
	}

	--this->m_uiReferences;
}


void CResourceMaterial::get_attribute_set(std::unordered_map<MaterialAttribute, float> & attribMap)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	attribMap = this->m_mAttribMap;
}