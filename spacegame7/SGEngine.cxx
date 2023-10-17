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
#include "SGEngine.hxx"

SGEngine::SGEngine(void)
{
	this->m_pPool = new PoolBlock[DEFAULT_POOL_SIZE];
	this->m_pPoolFlags = new PoolFlags[DEFAULT_POOL_SIZE];

	for(int i = 0; i < DEFAULT_POOL_SIZE; ++i)
	{
		this->m_pPoolFlags[i] = 0;
	}

	this->m_uiPoolPosition = 0u;
	this->m_uiAllocatedBlockCount = 0u;
}

SGEngine::~SGEngine(void)
{
	delete[] this->m_pPoolFlags;
	delete[] this->m_pPool;
}

/*
 * Request an instance from the engine.
 * Finds and returns the index of a free pool block.
 * 
 * This function does -not- invoke the constructor,
 * nor does it invoke instance_create(). These are responsibilities
 * of the caller. (see CInstanceFactory)
 * 
 * It is the CALLER's responsibility to ensure that they do not
 * request an object larger than the pool block size.
 * This is because the engine does not know what type it is performing
 * an allocation for, but the caller will, so it is possible to
 * perform a compile-time check instead of a runtime check.
 */
InstanceId SGEngine::instance_request(unsigned int const uiSize)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	bool bTraversedOnce = false;

ALLOC_SEARCH:
	//search for an unused pool block
	if(get_block_flag(this->m_pPoolFlags[this->m_uiPoolPosition], BLOCK_FLAG_ALLOCATED))
	{
		if (this->m_uiPoolPosition == DEFAULT_POOL_SIZE)
		{
			if (bTraversedOnce)
			{
				throw SGException("The object pool is full!");
			}
			else
			{
				bTraversedOnce = true;

				//if we've reached the end of the pool, just wrap to the beginning and
				//search for an open slot. chances are, we will find one. if not, an
				//exception will be thrown
				this->m_uiPoolPosition = 0;

				goto ALLOC_SEARCH_REEVALUATE;
			}
		}
	}
	else
	{
		goto ALLOC_SEARCH_CONCLUDED;
	}

	++this->m_uiPoolPosition;

	goto ALLOC_SEARCH;

ALLOC_SEARCH_REEVALUATE:
	if(bTraversedOnce && this->m_uiPoolPosition == 0)
	{
		goto ALLOC_SEARCH;
	}
ALLOC_SEARCH_CONCLUDED:

	//mark the block as allocated
	set_block_flag(this->m_pPoolFlags[this->m_uiPoolPosition], BLOCK_FLAG_ALLOCATED);

	++this->m_uiAllocatedBlockCount;
	return this->m_uiPoolPosition;
}

/*
 * Marks the associated pool block deallocated.
 * 
 * This function does -not- invoke instance_destroy() before
 * the free, nor does it invoke the destructor. These are
 * responsibilities of the caller. (see CInstanceFactory)
 */
void SGEngine::instance_free(InstanceId const iInstanceId)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(iInstanceId >= DEFAULT_POOL_SIZE || iInstanceId < 0)
	{
		throw SGException("Attempt to free out-of-bounds pool block");
	}

	if(!get_block_flag(this->m_pPoolFlags[iInstanceId], BLOCK_FLAG_ALLOCATED))
	{
		throw SGException("Attempt to free pool block not marked allocated");
	}

	IWorldInstance *pInstance = reinterpret_cast<IWorldInstance*>(&this->m_pPool[iInstanceId]);

	--this->m_uiAllocatedBlockCount;
	clear_block_flag(this->m_pPoolFlags[iInstanceId], BLOCK_FLAG_ALLOCATED);
}

/*
 * Get the memory pointer associated with a pool block index.
 */
IWorldInstance * SGEngine::instance_get(InstanceId const iInstanceId)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if (iInstanceId >= DEFAULT_POOL_SIZE || iInstanceId < 0)
	{
		throw SGException("Attempt to acquire out-of-bounds pool block");
	}

	if (!get_block_flag(this->m_pPoolFlags[iInstanceId], BLOCK_FLAG_ALLOCATED))
	{
		throw SGException("Attempt to acquire pool block not marked allocated");
	}

	IWorldInstance *pInstance = reinterpret_cast<IWorldInstance*>(&this->m_pPool[iInstanceId]);

	return pInstance;
}

/*
 * Get the number of allocated instances.
 */
unsigned int SGEngine::instance_get_count(void)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	return this->m_uiAllocatedBlockCount;
}

/*
 * This function simply returns whether the block given by a
 * certain instance ID is marked as allocated.
 */
bool SGEngine::instance_is_allocated(InstanceId const iInstanceId)
{
	if (iInstanceId == INVALID_INSTANCE)
	{
		return false;
	}

	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	return get_block_flag(this->m_pPoolFlags[iInstanceId], BLOCK_FLAG_ALLOCATED);
}


INLINE void SGEngine::set_block_flag(PoolFlags &blockFlags, PoolFlags const flags)
{
	blockFlags |= flags;
}

INLINE void SGEngine::clear_block_flag(PoolFlags &blockFlags, PoolFlags const flags)
{
	blockFlags &= ~flags;
}

INLINE bool SGEngine::get_block_flag(PoolFlags &blockFlags, PoolFlags const flags)
{
	return (blockFlags & flags) != 0;
}