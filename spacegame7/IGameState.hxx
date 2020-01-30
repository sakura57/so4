#pragma once

#include "SFMLIncludes.hxx"
#include "Util.hxx"

/*
 * Abstract representation of a game state
 * Contains functions for initialization,
 * termination, and a function which is
 * run on each tick of the render thread
 */
interface IGameState
{
	virtual ~IGameState() {};

	virtual void state_initializing(void) = 0;
	virtual void state_prerender_tick(sf::View &, sf::RenderWindow &, float const) = 0;
	virtual void state_render_world_tick(sf::View &, sf::RenderWindow &, float const) = 0;
	virtual void state_render_world_ui_tick(sf::View &, sf::RenderWindow &, float const) = 0;
	virtual void state_render_ui_tick(sf::View &, sf::RenderWindow &, float const) = 0;
	virtual void state_postrender_tick(sf::RenderWindow &, float const) = 0;
	virtual void state_process_event(sf::View &, sf::RenderWindow &, sf::Event &) = 0;
	virtual void state_terminating(void) = 0;
	virtual void state_enable_input(bool const) = 0;
	virtual void state_send_notification(std::string const &) {};

	virtual bool state_terminated(void) = 0;
	virtual bool state_game_paused(void) = 0;
};