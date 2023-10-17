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
#include "CLoadingScreenState.hxx"
#include "BaseHubPanel.hxx"

CLoadingScreenState::CLoadingScreenState()
{
}

CLoadingScreenState::~CLoadingScreenState()
{
}

void CLoadingScreenState::state_initializing(void)
{
	this->m_bInitialized = true;

	this->m_sfLoadingTextTexture.loadFromFile(CGameDataManager::get_full_data_file_path("graphics\\loadingscreen.png").c_str());
	this->m_sfLoadingText.setTexture(this->m_sfLoadingTextTexture);
	this->m_sfLoadingText.setScale(1.0f, 1.0f);
	this->m_sfLoadingText.setOrigin(256.0f, 256.0f);
}

void CLoadingScreenState::state_prerender_tick(sf::View&, sf::RenderWindow&, float const)
{
}

void CLoadingScreenState::state_render_world_tick(sf::View&, sf::RenderWindow&, float const)
{
}

void CLoadingScreenState::state_render_world_ui_tick(sf::View&, sf::RenderWindow&, float const)
{
}

void CLoadingScreenState::state_render_ui_tick(sf::View& sfView, sf::RenderWindow& sfWindow, float const flDelta)
{
	sf::Vector2u windowDimensions = sfWindow.getSize();
	sf::Vector2f textOrigin = this->m_sfLoadingText.getOrigin();
	sf::Vector2f loadingTextPosition((float)windowDimensions.x / 2.0f, (float)windowDimensions.y / 2.0f);

	this->m_sfLoadingText.setPosition(loadingTextPosition);

	sfWindow.draw(this->m_sfLoadingText);
}

void CLoadingScreenState::state_postrender_tick(sf::RenderWindow&, float const)
{
}

void CLoadingScreenState::state_terminating(void)
{
}

void CLoadingScreenState::state_process_event(sf::View&, sf::RenderWindow&, sf::Event&)
{
}

bool CLoadingScreenState::state_terminated(void)
{
	return false;
}

bool CLoadingScreenState::state_game_paused(void)
{
	return true;
}

bool CLoadingScreenState::state_render_world(void)
{
	return false;
}

bool CLoadingScreenState::state_render_ui(void)
{
	return false;
}