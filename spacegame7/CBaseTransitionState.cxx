#include "BaseEnterPanel.hxx"
#include "CBaseTransitionState.hxx"

CBaseTransitionState::CBaseTransitionState(BaseId const baseId)
	: m_iBase(baseId)
{
}

CBaseTransitionState::~CBaseTransitionState()
{
}

void CBaseTransitionState::state_initializing(void)
{
	SG::get_interface_manager()->add_panel(new BaseEnterPanel(this->m_iBase));

	this->m_bInitialized = true;
}

void CBaseTransitionState::state_prerender_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CBaseTransitionState::state_render_world_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CBaseTransitionState::state_render_world_ui_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CBaseTransitionState::state_render_ui_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CBaseTransitionState::state_postrender_tick(sf::RenderWindow &, float const)
{
}

void CBaseTransitionState::state_terminating(void)
{
}

void CBaseTransitionState::state_process_event(sf::View &, sf::RenderWindow &, sf::Event &)
{
}

bool CBaseTransitionState::state_terminated(void)
{
	return false;
}
