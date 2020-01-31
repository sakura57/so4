#pragma once

#include "IGameState.hxx"
#include "IEngine.hxx"
#include "IRenderPipeline.hxx"
#include "IScriptEngine.hxx"
#include "IWorld.hxx"
#include "ICamera.hxx"
#include "CGameDataManager.hxx"
#include "CShip.hxx"
#include "InterfaceManager.hxx"
#include "CAudioManager.hxx"
#include "IUniverse.hxx"

class CSectorTransitionState : public IGameState
{
public:
	CSectorTransitionState(SectorId const, std::string const&, Vector2f const &);
	~CSectorTransitionState();

	virtual void state_initializing(void) final;
	virtual void state_prerender_tick(sf::View &, sf::RenderWindow &, float const) final;
	virtual void state_render_world_tick(sf::View &, sf::RenderWindow &, float const) final;
	virtual void state_render_world_ui_tick(sf::View &, sf::RenderWindow &, float const) final;
	virtual void state_render_ui_tick(sf::View &, sf::RenderWindow &, float const) final;
	virtual void state_postrender_tick(sf::RenderWindow &, float const) final;

	virtual void state_terminating(void) final;
	virtual void state_process_event(sf::View &, sf::RenderWindow &, sf::Event &) final;

	virtual bool state_terminated(void) final;
	virtual bool state_game_paused(void) final;

	virtual bool state_render_world(void) final;
	virtual bool state_render_ui(void) final;

	virtual void state_enable_input(bool const) final
	{
	};

private:
	bool m_bInitialized;
	SectorId m_iSector;

	Spinlock m_mFieldAccess;

	IEngine * m_pEngine;
	IRenderPipeline * m_pRenderPipeline;
	IWorld * m_pWorld;
	IScriptEngine *m_pScriptEngine;
	CGameDataManager *m_pGameDataManager;
	InterfaceManager *m_pInterfaceManager;
	CAudioManager *m_pAudioManager;
	CParticleManager *m_pParticleManager;
	CCommsManager *m_pCommsManager;
	IUniverse *m_pUniverse;

	ICamera * m_pActiveCamera;

	Vector2f m_vStartingPosition;
	std::string m_szRmsnScript;
};