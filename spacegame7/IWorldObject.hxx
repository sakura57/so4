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

#include <memory>
#include "Defs.hxx"
#include "EngineProtos.hxx"
#include "Util.hxx"

#include "IWorldInstance.hxx"
#include "IPhysicalObject.hxx"
#include "IRadarVisible.hxx"

typedef unsigned int AppearanceId;
#define APPEARANCE_NONE 0U

struct RenderParameters
{
	Vector2f vScale;
	float fAlpha;
};

/*
 * IWorldObject represents a world instance
 * which possesses location attributes
 * and an appearance.
 */
interface IWorldObject : public IWorldInstance
{
	enum
	{
		InstanceFlag = 1 << 1
	};

	virtual ~IWorldObject(void);
	
	virtual Vector2f get_position(void) = 0;
	virtual void set_position(Vector2f const &) = 0;
	virtual float get_rotation(void) = 0;
	virtual void set_rotation(float const) = 0;
	virtual AppearanceId get_appearance(void) = 0;
	virtual void set_appearance(AppearanceId) = 0;
	virtual bool is_initialized(void) = 0;

	virtual void get_render_parms(RenderParameters &renderParms)
	{
		renderParms.vScale = Vector2f(1.0f, 1.0f);
		renderParms.fAlpha = 255;
	};

	/*
	 * by default, world objects will return a null
	 * physics interface. (no physics)
	 * 
	 * derived classes which implement
	 * IPhysicalObject must override this function
	 * and return their physics interface
	 */
	virtual IPhysicalObject *get_physical(void)
	{
		return nullptr;
	};

	virtual IRadarVisible *get_radar(void)
	{
		return nullptr;
	};
};
