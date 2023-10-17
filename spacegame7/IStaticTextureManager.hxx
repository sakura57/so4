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

#include "Util.hxx"

typedef int StaticTextureId;

//Hard limit on the number of static textures to prevent memory leaks
#define MAXIMUM_STATIC_TEXTURES 1024

struct StaticTextureRenderParameters
{
	float flPositionX, flPositionY, flPositionZ;
	float flWidth, flHeight;
	float flRotation;
};

/*
 * Provides an interface for a static texture manager.
 * 
 * The interface should support the ability to load a texture from an image file, and the ability
 * to render a texture as a quad at a particular position in 3D space.
 */
interface IStaticTextureManager
{
	virtual void load_texture(StaticTextureId const, char const *) = 0;
	virtual void render_texture(StaticTextureId const, StaticTextureRenderParameters const &) = 0;
	virtual void *get_texture_handle(StaticTextureId const) = 0;
};