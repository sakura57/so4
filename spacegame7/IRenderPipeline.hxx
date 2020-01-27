#pragma once

#include "Defs.hxx"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "ICamera.hxx"
#include "IWorldObject.hxx"

#include "ISectorMapRenderer.hxx"

/*
	The render pipeline is responsible for
	managing the window, graphics resources (images, sprites)
	and providing methods for drawing to the screen.
*/
interface IRenderPipeline
{
	struct ImageOptions
	{
		AppearanceId uiAppearanceId;
		Vector2f vCenter;
		Vector2f vScale;
		bool bAdditive;
	};

	//renders an object on the screen, using
	//the object's appearance id to index its
	//sprite
	virtual void render_object(IWorldObject *, sf::RenderWindow &, RenderParameters const &) = 0;

	//Renders a particular appearance ID
	virtual void render_image(AppearanceId const, sf::RenderWindow &, Vector2f const &, float const, RenderParameters const &) = 0;

	//detaches from the active camera, so that
	//set_active_camera() may be invoked with a fresh
	//camera.
	virtual void remove_active_camera(void) = 0;

	//returns the active camera.
	virtual ICamera *acquire_active_camera(void) = 0;

	virtual void release_active_camera(void) = 0;

	//specify the camera from whose perspective
	//we will be drawing world objects
	virtual void set_active_camera(ICamera *) = 0;

	//specify the desired appearance id for the image,
	//and the image filename. success is implicit
	//unless an exception is thrown
	virtual void load_image(ImageOptions const &, char const *) = 0;

	//return true if objects may be rendered at this time.
	//usually will return false if there is no active camera,
	//in which case objects should not be rendered.
	virtual bool can_render_objects(void) = 0;

	//get the sector map renderer (implementation dependent
	//on graphics implementation)
	virtual ISectorMapRenderer* get_sector_map_renderer(void) = 0;
};