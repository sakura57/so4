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

#include "IEngine.hxx"
#include "SGEngine.hxx"
#include "SGLib.hxx"

/*
 * CInstanceFactory
 * 
 * Base factory of IWorldInstance-based objects.
 * Memory allocation is performed by the engine.
 */
class CInstanceFactory
{
public:
	//factory only provides methods; no instantiation
	CInstanceFactory() = delete;
	CInstanceFactory(CInstanceFactory const &) = delete;
	~CInstanceFactory() = delete;

	/*
	 * create an instance of class T.
	 * the template parameter may NOT be a base class
	 * or interface.
	 * 
	 * This function invokes instance_request(),
	 * which may throw an SGException if allocation fails,
	 * for instance, due to the pool being full.
	 * 
	 * A compile-time error will be generated if a
	 * type has been supplied to the template that
	 * cannot be allocated.
	 */
	template<class T>
	static void create(InstanceId &iInstance, T *&pInstance)
	{
		//we shouldn't allow pool objects of size zero
		static_assert(sizeof(T) > 0, "Pool does not permit objects of size zero");

		//pool objects are limited in size. this is enforceable
		//at compile-time
		static_assert(sizeof(T) <= DEFAULT_POOL_BLOCK_SIZE, "Object size is greater than pool block size");

		IEngine *engine = SG::get_engine();

		iInstance = engine->instance_request(sizeof(T));

		//placement new will ensure the correct constructor is invoked
		pInstance = new(reinterpret_cast<T*>(engine->instance_get(iInstance))) T();
		
		//give the instance a chance to execute its initialization code
		pInstance->instance_create(iInstance);
	};

	/*
	 * Frees an instance.
	 * 
	 * Invokes instance_destroy(), so an SGException will be
	 * thrown if an unfreeable instance ID is supplied.
	 */
	static void destroy(InstanceId const iInstance)
	{
		IEngine *engine = SG::get_engine();

		IWorldInstance *pInstance = engine->instance_get(iInstance);

		//invoke instance_destroy
		pInstance->instance_destroy(iInstance);

		//no need for placement delete.
		//since IWorldInstance's destructor is virtual,
		//invocation will invoke the correct subclass destructor
		pInstance->~IWorldInstance();

		//mark the block of memory as freed
		engine->instance_free(iInstance);
	};
};