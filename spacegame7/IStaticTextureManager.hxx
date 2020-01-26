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