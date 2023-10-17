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
#include "CStaticCamera.hxx"

CStaticCamera::CStaticCamera(void)
	: m_vTarget(0.0f, 0.0f)
{
}

CStaticCamera::~CStaticCamera(void)
{
}

void CStaticCamera::set_target_point(Vector2f const &tv)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vTarget = tv;
	this->m_vPosition = tv;
}

void CStaticCamera::movement_update(float const)
{
	//do nothing; our position is already set by set_target_point
}
