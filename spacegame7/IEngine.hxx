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
#include "IWorldObject.hxx"

/*
 * The core engine interface.
 * 
 * Provides functions for quickly allocating and
 * freeing world instances.
 */
interface IEngine
{
	virtual ~IEngine(void) { };

	//request an instance of a particular class from the pool
	virtual InstanceId instance_request(unsigned int const) = 0;

	//get a pointer to the block of memory associated with a
	//particular instance ID
	virtual IWorldInstance * instance_get(InstanceId const) = 0;

	//frees the block of memory associated with the particular instance
	virtual void instance_free(InstanceId const) = 0;

	//return the number of allocated instances
	virtual unsigned int instance_get_count(void) = 0;

	//can the instance safely be acquired?
	virtual bool instance_is_allocated(InstanceId const) = 0;

	template<class T>
	T *instance_get_checked(InstanceId const iInstanceId)
	{
		if(!this->instance_is_allocated(iInstanceId))
		{
			return nullptr;
		}

		IWorldInstance *pInstance = this->instance_get(iInstanceId);

		if(pInstance->instance_get_flags() & T::InstanceFlag)
		{
			return static_cast<T*>(pInstance);
		}

		return nullptr;
	};
};