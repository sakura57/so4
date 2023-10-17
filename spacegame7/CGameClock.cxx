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
#include "CGameClock.hxx"

CGameClock::CGameClock()
	: m_flAbsoluteDelta(0.0f)
{

}

CGameClock::~CGameClock()
{

}

void CGameClock::tick(float const flDelta)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flAbsoluteDelta += flDelta;
}

float CGameClock::get_absolute_delta(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_flAbsoluteDelta;
}