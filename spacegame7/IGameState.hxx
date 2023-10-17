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
	virtual bool state_render_world(void) = 0;
	virtual bool state_render_ui(void) = 0;
};