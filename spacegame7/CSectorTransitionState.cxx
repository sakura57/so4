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
#include "SectorEnterPanel.hxx"
#include "CSectorTransitionState.hxx"

CSectorTransitionState::CSectorTransitionState(SectorId const sectorId, std::string const &szRmsnScript, Vector2f const &vStartingPosition)
	: m_iSector(sectorId), m_vStartingPosition(vStartingPosition), m_szRmsnScript(szRmsnScript)
{
}

CSectorTransitionState::~CSectorTransitionState()
{
}

void CSectorTransitionState::state_initializing(void)
{
	SG::get_interface_manager()->add_panel(new SectorEnterPanel(this->m_iSector, this->m_szRmsnScript, this->m_vStartingPosition));

	this->m_bInitialized = true;
}

void CSectorTransitionState::state_prerender_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CSectorTransitionState::state_render_world_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CSectorTransitionState::state_render_world_ui_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CSectorTransitionState::state_render_ui_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CSectorTransitionState::state_postrender_tick(sf::RenderWindow &, float const)
{
}

void CSectorTransitionState::state_terminating(void)
{
}

void CSectorTransitionState::state_process_event(sf::View &, sf::RenderWindow &, sf::Event &)
{
}

bool CSectorTransitionState::state_terminated(void)
{
	return false;
}

bool CSectorTransitionState::state_game_paused(void)
{
	return false;
}

bool CSectorTransitionState::state_render_world(void)
{
	return true;
}

bool CSectorTransitionState::state_render_ui(void)
{
	return true;
}