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

#include "Defs.hxx"
#include "Util.hxx"
#include "EngineProtos.hxx"
#include "IWorldInstance.hxx"

/*
* Implementation of the base world object,
* possessing location and physical properties.
*
* Thread-safe, because an object may be accessed
* from multiple threads (rendering thread, physics thread)
*
* NOTE: Since Verlet does not explicitly track velocities,
* set_velocity and set_angular_velocity in CObject and all
* derivatives have no effect. However, getters should still
* work properly.
*/
class CNonPhysical : public IWorldInstance
{
public:
	enum
	{
		InstanceFlag = 1 << 9
	};

	struct CreationParameters
	{ };

	CNonPhysical(void);

	//members inherited from IWorldInstance
	virtual void instance_create(InstanceId const);
	virtual void instance_destroy(InstanceId const);
	virtual InstanceId instance_get_id(void) final;
	virtual InstanceFlags instance_get_flags(void) final;
	virtual void mark_for_deletion(void) final;
	virtual bool is_marked_for_deletion(void) final;
	virtual void instance_acquired(void) final;
	virtual void instance_released(void) final;
	virtual bool is_safely_destroyable(void) final;
	virtual ~CNonPhysical(void);

	//members defined in CNonPhysical
	virtual void initialize(CreationParameters const &);

protected:
	Spinlock m_mFieldAccess;

	unsigned int m_uiReferenceCount;

	bool m_bMarkedForDeletion;
	InstanceId m_iInstanceId;
	InstanceFlags m_uiInstanceFlags;
};
