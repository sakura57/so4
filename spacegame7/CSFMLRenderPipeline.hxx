#pragma once

#include <mutex>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "IRenderPipeline.hxx"

class CSFMLRenderPipeline : public IRenderPipeline, public CListener
{
public:
	CSFMLRenderPipeline();
	~CSFMLRenderPipeline();

	//renders an object on the screen, using
	//the object's appearance id to index its
	//sprite
	virtual void render_object(IWorldObject *, sf::RenderWindow &, RenderParameters const &) final;

	//renders an object on the screen, given a 
	//specific appearance ID
	virtual void render_image(AppearanceId const, sf::RenderWindow &, Vector2f const &, float const, RenderParameters const &) final;

	//return the active camera. member field access is locked,
	//when the caller is done with the camera, they must invoke
	//release_active_camera()
	//if this function returns null, there is no active camera,
	//and release_active_camera should not be called, or an exception
	//will be raised
	virtual ICamera * acquire_active_camera(void) final;

	//should always be called when a caller of acquire_active_camera()
	//is finished with the camera
	virtual void release_active_camera(void) final;

	//specify the camera from whose perspective
	//we will be drawing world objects
	virtual void set_active_camera(ICamera *) final;

	//detaches from the active camera, so that
	//set_active_camera() may be invoked with a fresh
	//camera.
	virtual void remove_active_camera(void) final;

	//specify the desired appearance id for the image,
	//and the image filename. success is implicit
	//unless an exception is thrown
	virtual void load_image(ImageOptions const &, char const *) final;

	//return true if objects may be rendered at this time.
	//usually will return false if there is no active camera,
	//in which case objects should not be rendered.
	virtual bool can_render_objects(void) final;

	//we expect to receive notifications from dependents
	//(e.g. if the active camera is destroyed while still active)
	virtual void listen_notify(Notification const, IListenable *) final;

	//get the sector map renderer (implementation dependent
	//on graphics implementation)
	virtual ISectorMapRenderer* get_sector_map_renderer(void) final;

private:
	std::mutex m_mFieldAccess;
	ICamera *m_pActiveCamera;
	ISectorMapRenderer* m_pSectorMapRenderer;
	AppearanceId m_uiPastAllocatedAppearances;
	std::vector<sf::Texture> m_vecTextures;
	std::vector<sf::Sprite> m_vecSprites;
	std::vector<sf::Vector2f> m_vecScales;
	std::vector<bool> m_vecAppearanceValid;
	std::vector<bool> m_vecAppearanceAdditive;
	//sf::Shader m_blurShader;
};