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
#include "CStaticObject.hxx"

CStaticObject::CStaticObject(StaticTextureId const iStaticTextureId)
{
	this->m_iStaticTextureId = iStaticTextureId;
}

CStaticObject::CStaticObject(StaticTextureId const iStaticTextureId,
	float const flPositionX, float const flPositionY, float const flPositionZ,
	float const flRotation,
	Vector2f const &vBounds)
	: m_iStaticTextureId(iStaticTextureId),
	m_flPositionX(flPositionX),
	m_flPositionY(flPositionY),
	m_flPositionZ(flPositionZ),
	m_flRotation(flRotation),
	m_vBounds(vBounds)
{

}

CStaticObject::~CStaticObject()
{
	
}

StaticTextureId CStaticObject::get_texture_id(void) const
{
	return this->m_iStaticTextureId;
}

void CStaticObject::set_position(float const flPositionX, float const flPositionY, float const flPositionZ)
{
	this->m_flPositionX = flPositionX;
	this->m_flPositionY = flPositionY;
	this->m_flPositionZ = flPositionZ;
}

void CStaticObject::get_position(float &flPositionX, float &flPositionY, float &flPositionZ) const
{
	flPositionX = this->m_flPositionX;
	flPositionY = this->m_flPositionY;
	flPositionZ = this->m_flPositionZ;
}

void CStaticObject::set_rotation(float const flRotation)
{
	this->m_flRotation = flRotation;
}

void CStaticObject::get_rotation(float &flRotation) const
{
	flRotation = this->m_flRotation;
}

void CStaticObject::set_bounds(Vector2f const &vBounds)
{
	this->m_vBounds = vBounds;
}

void CStaticObject::get_bounds(Vector2f &vBounds) const
{
	vBounds = this->m_vBounds;
}