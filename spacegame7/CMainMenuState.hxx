#pragma once

#include "SFMLIncludes.hxx"
#include "IGameState.hxx"

class CMainMenuState : public IGameState
{
public:
	CMainMenuState();
	~CMainMenuState();

	virtual void state_initializing(void) final;
	virtual void state_prerender_tick(sf::View &, sf::RenderWindow &, float const) final;
	virtual void state_render_world_tick(sf::View &, sf::RenderWindow &, float const) final;
	virtual void state_render_world_ui_tick(sf::View &, sf::RenderWindow &, float const) final;
	virtual void state_render_ui_tick(sf::View &, sf::RenderWindow &, float const) final;
	virtual void state_postrender_tick(sf::RenderWindow &, float const) final;
	virtual void state_terminating(void) final;
	virtual void state_process_event(sf::View &, sf::RenderWindow &, sf::Event &) final;

	virtual void state_enable_input(bool const) final
	{
	};

	virtual bool state_terminated(void) final;

	virtual bool state_game_paused(void) final;

	virtual bool state_render_world(void) final;
	virtual bool state_render_ui(void) final;

private:
	sf::VertexArray m_sfBgVerts;
	sf::Shader m_sfBackgroundShader;

	unsigned int m_uiGraphicsQuality;
	float m_flTimeElapsed;
	bool m_bInitialized;
};