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

#include "IStaticTextureManager.hxx"

class CStaticObject
{
public:
	CStaticObject(StaticTextureId const);
	CStaticObject(StaticTextureId const, float const, float const, float const, float const, Vector2f const &);
	~CStaticObject();

	StaticTextureId get_texture_id(void) const;

	void set_position(float const, float const, float const);
	void get_position(float &, float &, float &) const;

	void set_rotation(float const);
	void get_rotation(float &) const;

	void set_bounds(Vector2f const &);
	void get_bounds(Vector2f &) const;

private:
	StaticTextureId m_iStaticTextureId;
	float m_flPositionX, m_flPositionY, m_flPositionZ;
	float m_flRotation;
	Vector2f m_vBounds;
};