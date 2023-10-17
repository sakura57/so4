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
#include "CVignetteCamera.hxx"

CVignetteCamera::CVignetteCamera(void)
	: m_vStartPoint(0.0f, 0.0f), m_vEndPoint(0.0f, 0.0f), m_flTimeElapsed(0.0f), m_flPathDuration(5.0f)
{
}

CVignetteCamera::~CVignetteCamera(void)
{
}

void CVignetteCamera::set_origin_point(Vector2f const& tv)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vStartPoint = tv;
	this->m_vPosition = tv;
}

void CVignetteCamera::set_final_point(Vector2f const& tv)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vEndPoint = tv;
}

void CVignetteCamera::set_path_duration(float const flDuration)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flPathDuration = flDuration;
	this->m_flTimeElapsed = 0.0f;
}

void CVignetteCamera::movement_update(float const flDelta)
{
	this->m_flTimeElapsed += flDelta;

	Vector2f delta = this->m_vEndPoint - this->m_vStartPoint;

	float flShiftAmount = clamp(this->m_flTimeElapsed / this->m_flPathDuration, 0.0f, 1.0f);

	Vector2f shift = delta * flShiftAmount;

	this->m_vPosition = this->m_vStartPoint + shift;
}
