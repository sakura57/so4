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

#include <mutex>
#include <vector>

#include "IEngine.hxx"
#include "IWorldInstance.hxx"

#define DEFAULT_POOL_BLOCK_SIZE 4096
#define DEFAULT_POOL_SIZE 8192

typedef unsigned char PoolBlock[DEFAULT_POOL_BLOCK_SIZE];
typedef unsigned int PoolFlags;

#define BLOCK_FLAG_ALLOCATED (1 << 0)

/*
 * Engine implementation.
 * 
 * Uses a pool scheme for instance management.
 */
class SGEngine : public IEngine
{
public:
	SGEngine(void);
	virtual ~SGEngine(void);

	virtual InstanceId instance_request(unsigned int const) final;
	virtual void instance_free(InstanceId const) final;
	virtual IWorldInstance * instance_get(InstanceId const) final;
	virtual unsigned int instance_get_count(void) final;
	virtual bool instance_is_allocated(InstanceId const) final;

private:
	INLINE static bool get_block_flag(PoolFlags &, PoolFlags const);
	INLINE static void set_block_flag(PoolFlags &, PoolFlags const);
	INLINE static void clear_block_flag(PoolFlags &, PoolFlags const);

	std::mutex m_mFieldAccess;
	unsigned int m_uiPoolPosition;
	unsigned int m_uiAllocatedBlockCount;
	PoolFlags *m_pPoolFlags;
	PoolBlock *m_pPool;
};
