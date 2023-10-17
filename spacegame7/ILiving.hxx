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

/*
 * Represents a "living" object, which may be enabled
 * or disabled. alive_tick() is invoked each tick,
 * usually after physics_tick() for physical objects.
 */
interface ILiving
{
    enum
	{
		InstanceFlag = 1 << 4
	};
    
	//to prevent a dynamic_cast issue
	virtual ~ILiving();

	/*
	 * any needed behaviour should be implemented
	 * here. keep in mind, if the object is dead,
	 * custom behaviour should not be executed
	 * and alive_tick() should have no effect.
	 */
	virtual void alive_tick(float const) = 0;

	/*
	 * Mark this object either alive or dead.
	 * If the object is dead, alive_tick() should
	 * have no effect.
	 */
	virtual void alive_set(bool const) = 0;

	/*
	 * Return whether this object is alive.
	 */
	virtual bool is_alive(void) = 0;
};
