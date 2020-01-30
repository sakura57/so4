#include "CMainMenuState.hxx"
#include "SGLib.hxx"
#include "MainMenuPanel.hxx"

CMainMenuState::CMainMenuState()
	: m_sfBgVerts(sf::TrianglesStrip, 5)
{
}

CMainMenuState::~CMainMenuState()
{
}

void CMainMenuState::state_initializing(void)
{
	this->m_uiGraphicsQuality = 0;
	this->m_flTimeElapsed = 0.0f;
	SG::get_game_settings()->get_setting<unsigned int>("quality_level", this->m_uiGraphicsQuality);

	this->m_sfBackgroundShader.loadFromFile(CGameDataManager::get_full_data_file_path("shaders\\mainmenu_bg.txt").c_str(), sf::Shader::Fragment);
	this->m_sfBgVerts[0].position = sf::Vector2f(DEFAULT_WINDOW_WIDTH, 0);
	this->m_sfBgVerts[1].position = sf::Vector2f(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	this->m_sfBgVerts[2].position = sf::Vector2f(0, DEFAULT_WINDOW_HEIGHT);
	this->m_sfBgVerts[3].position = sf::Vector2f(0, 0);
	this->m_sfBgVerts[4].position = sf::Vector2f(DEFAULT_WINDOW_WIDTH, 0);

	SG::get_interface_manager()->add_panel(new MainMenuPanel);

	this->m_bInitialized = true;
}

void CMainMenuState::state_prerender_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CMainMenuState::state_render_world_tick(sf::View &sfView, sf::RenderWindow &sfWindow, float const flDelta)
{
	this->m_flTimeElapsed += flDelta;

	if(this->m_uiGraphicsQuality > 3)
	{
		sf::RenderStates states;
		states.shader = &this->m_sfBackgroundShader;

		sf::Vector2f vCameraPosition = sfView.getCenter();

		this->m_sfBackgroundShader.setParameter("resolution", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
		this->m_sfBackgroundShader.setParameter("time", this->m_flTimeElapsed);

		sfWindow.draw(this->m_sfBgVerts, states);
	}
}

void CMainMenuState::state_render_world_ui_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CMainMenuState::state_render_ui_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CMainMenuState::state_postrender_tick(sf::RenderWindow &, float const)
{
}

void CMainMenuState::state_terminating(void)
{
}

void CMainMenuState::state_process_event(sf::View &, sf::RenderWindow &, sf::Event &)
{
}

bool CMainMenuState::state_terminated(void)
{
	return this->m_bInitialized && SG::get_interface_manager()->get_num_panels() == 0;
}

bool CMainMenuState::state_game_paused(void)
{
	return false;
}