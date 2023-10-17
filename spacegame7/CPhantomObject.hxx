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
#include "IWorldObject.hxx"
#include "SGLib.hxx"

/*
 * A phantom is an object possessing location attributes
 * but no appearance.
 */
class CPhantomObject : public IWorldObject
{
public:
	enum
	{
		InstanceFlag = 1 << 17
	};

	struct CreationParameters
	{
		AppearanceId uiAppearanceId;
	};

	CPhantomObject(void);

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

	//members inherited from IWorldObject
	virtual ~CPhantomObject(void);
	virtual Vector2f get_position(void);
	virtual void set_position(Vector2f const &);
	virtual float get_rotation(void);
	virtual void set_rotation(float const);
	virtual AppearanceId get_appearance(void);
	virtual void set_appearance(AppearanceId);
	virtual bool is_initialized(void);

	//members defined in CPhantomObject
	virtual void initialize(CreationParameters const &);

	virtual void get_render_parms(RenderParameters &renderParms)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		renderParms.vScale = Vector2f(1.0f, 1.0f);
		renderParms.fAlpha = 0.0f;
	};

protected:
	//since CPhantomObject's functions are not time-consuming,
	//mutual exclusion is provided by a spinlock.
	Spinlock m_mFieldAccess;

	bool m_bInitialized;

	unsigned int m_uiReferenceCount;

	bool m_bMarkedForDeletion;
	InstanceId m_iInstanceId;
	InstanceFlags m_uiInstanceFlags;

	AppearanceId m_uiAppearanceId;

private:
};