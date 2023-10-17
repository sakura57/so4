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

#include "IGameClock.hxx"
#include "Util.hxx"

/*
 * Implementation of game clock class.
 * This is instantiated as a static global in SGLib
 * while tick() is called each frame by the render thread.
 *
 * get_absolute_delta() can be called at any time, from any thread
 * returning the time passed since this class was instantiated,
 * which is usually around the time the game was started.
 */
class CGameClock : public IGameClock
{
public:
	CGameClock();
	~CGameClock();

	virtual void tick(float const);
	virtual float get_absolute_delta(void);

private:
	Spinlock m_mFieldAccess;
	float m_flAbsoluteDelta;
};