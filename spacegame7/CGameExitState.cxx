#include "CGameExitState.hxx"
#include "BaseHubPanel.hxx"

CGameExitState::CGameExitState()
{
}

CGameExitState::~CGameExitState()
{
}

void CGameExitState::state_initializing(void)
{
	this->m_bInitialized = true;
}

void CGameExitState::state_prerender_tick(sf::View&, sf::RenderWindow&, float const)
{
}

void CGameExitState::state_render_world_tick(sf::View&, sf::RenderWindow&, float const)
{
}

void CGameExitState::state_render_world_ui_tick(sf::View&, sf::RenderWindow&, float const)
{
}

void CGameExitState::state_render_ui_tick(sf::View&, sf::RenderWindow&, float const)
{
}

void CGameExitState::state_postrender_tick(sf::RenderWindow&, float const)
{
}

void CGameExitState::state_terminating(void)
{
}

void CGameExitState::state_process_event(sf::View&, sf::RenderWindow&, sf::Event&)
{
}

bool CGameExitState::state_terminated(void)
{
	return true;
}

bool CGameExitState::state_game_paused(void)
{
	return false;
}

bool CGameExitState::state_render_world(void)
{
	return true;
}

bool CGameExitState::state_render_ui(void)
{
	return true;
}