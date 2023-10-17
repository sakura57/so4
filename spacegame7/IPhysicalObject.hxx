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
#include "EngineProtos.hxx"
#include "IWorldInstance.hxx"
#include "IWorldObject.hxx"

//negative radius indicates collisions are disabled
#define RADIUS_ZERO -1.0f

/*
 * Represents a physics-enabled object,
 * with velocity, acceleration, radius.
 */
interface IPhysicalObject
{
	enum
	{
		InstanceFlag = 1 << 2
	};

	enum PhysicsType : unsigned int
	{
		NonCollidable = 0,
		Collidable = 1 << 0, //collisions are resolved
		HasCallback = 1 << 1, //collision_callback invoked on collisions
		Solid = 1 << 2, //collisions correct positions and velocity
		Immovable = 1 << 3 //collisions correct for the other colliding body
	};

	IPhysicalObject(void) = default;

	virtual ~IPhysicalObject() { };

	virtual void physics_tick(float const) = 0;

	virtual Vector2f get_velocity(void) = 0;
	virtual void set_velocity(Vector2f const &) = 0;
	virtual float get_angular_velocity(void) = 0;
	virtual void set_angular_velocity(float const) = 0;
	virtual Vector2f get_acceleration(void) = 0;
	virtual void set_acceleration(Vector2f const &) = 0;
	virtual float get_angular_acceleration(void) = 0;
	virtual void set_angular_acceleration(float const) = 0;

	virtual unsigned int get_physics_type(void) const = 0;

	virtual float get_mass(void) = 0;
	virtual void set_mass(float const) = 0;
	virtual float get_rotational_inertia(void) = 0;
	virtual void set_rotational_inertia(float const) = 0;

	virtual float get_radius(void) = 0;
	virtual void set_radius(float const) = 0;

	/*
	 * callback invoked upon collision.
	 * the callback is invoked immediately post-resolve
	 * meaning the objects (hopefully) aren't touching
	 * any more, although they might be within a tick
	 * or two
	 */
	virtual void collision_callback(IWorldObject *) = 0;
};
