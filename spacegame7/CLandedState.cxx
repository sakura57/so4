#include "CLandedState.hxx"
#include "BaseHubPanel.hxx"

CLandedState::CLandedState(BaseId const iBaseId)
	: m_iBaseId(iBaseId)
{
}

CLandedState::~CLandedState()
{
}

void CLandedState::state_initializing(void)
{
	SG::get_interface_manager()->add_panel(new BaseHubPanel(this->m_iBaseId));

	this->m_bInitialized = true;
}

void CLandedState::state_prerender_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CLandedState::state_render_world_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CLandedState::state_render_world_ui_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CLandedState::state_render_ui_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CLandedState::state_postrender_tick(sf::RenderWindow &, float const)
{
}

void CLandedState::state_terminating(void)
{
}

void CLandedState::state_process_event(sf::View &, sf::RenderWindow &, sf::Event &)
{
}

bool CLandedState::state_terminated(void)
{
	return false;
}

bool CLandedState::state_game_paused(void)
{
	return false;
}

bool CLandedState::state_render_world(void)
{
	return true;
}

bool CLandedState::state_render_ui(void)
{
	return true;
}