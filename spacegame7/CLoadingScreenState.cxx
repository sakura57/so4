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

void CLoadingScreenState::state_render_ui_tick(sf::View&, sf::RenderWindow&, float const)
{
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