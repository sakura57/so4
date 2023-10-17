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

typedef long InstanceId;
typedef unsigned int InstanceFlags;
#define INVALID_INSTANCE -1L

/*
 * All objects are assigned an instance ID by the engine.
 * The IWorldInstance interface provides access to the instance ID.
 * 
 * In addition, instance_create is invoked by the engine
 * upon creation, and instance_destroy is invoked when the object
 * is marked as dead.
 */
interface IWorldInstance
{
	enum
	{
		InstanceFlag = 1 << 0
	};

	virtual ~IWorldInstance(void) { };

	//instance_create is invoked after object allocation,
	//immediately after the constructor
	virtual void instance_create(InstanceId const) = 0;

	//instance_destroy is invoked immediately before destruction
	virtual void instance_destroy(InstanceId const) = 0;

	//return this object's instance id
	virtual InstanceId instance_get_id(void) = 0;

	//return this object's instance flags
	//instance flags should be set by instance_create and -not modified-
	virtual InstanceFlags instance_get_flags(void) = 0;
    
    //invoke to mark this instance for deletion, in which
    //case it will be deleted during the next tick
    virtual void mark_for_deletion(void) = 0;
    
    //return whether this instance is marked for deletion
    virtual bool is_marked_for_deletion(void) = 0;

	//increment reference counter
	virtual void instance_acquired(void) = 0;

	//decrement reference counter
	virtual void instance_released(void) = 0;

	//should return true if this instance's reference count
	//is at zero, or if the instance is always safely destroyable
	virtual bool is_safely_destroyable(void) = 0;
};
