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
#include "IWorldObject.hxx"
#include "Listeners.hxx"

/*
 * Basic camera interface, encapsulating
 * world bounds, zoom, and projection.
 */
interface ICamera : public IWorldObject, public CListenable
{
    enum
	{
		InstanceFlag = 1 << 5
	};
    
	/*
	* The camera's bounds are the width and height,
	* in world coordinates, which will be visible on-screen.
	*/
	virtual Vector2f get_bounds(void) = 0;

	/*
	 * alter this camera's world bounds
	 */
	virtual void set_bounds(Vector2f const &) = 0;

	/*
	 * the zoom factor represents the ratio of world
	 * bounds to screen dimensions
	 */
	virtual float get_zoom_factor(void) = 0;

	/*
	 * alter the bounds in such a way that the specified
	 * zoom factor is achieved.
	 */
	virtual void set_zoom_factor(float const) = 0;

	/*
	 * width and height of the window or screen
	 */
	virtual Vector2f get_screen_dimensions(void) = 0;

	/*
	 * the camera should be notified if a change of screen
	 * dimensions occurs.
	 */
	virtual void set_screen_dimensions(Vector2f const &) = 0;

	/*
	 * given world coordinates, determine screen coordinates.
	 * returns whether the point is on the screen or not
	 */
	virtual bool project(Vector2f const &, Vector2f &) = 0;
};
