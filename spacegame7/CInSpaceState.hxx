#pragma once

#include "Defs.hxx"
#include "IGameState.hxx"
#include "IEngine.hxx"
#include "IRenderPipeline.hxx"
#include "IScriptEngine.hxx"
#include "IWorld.hxx"
#include "ICamera.hxx"
#include "IStaticTextureManager.hxx"
#include "CGameDataManager.hxx"
#include "CShip.hxx"
#include "InterfaceManager.hxx"
#include "CAudioManager.hxx"
#include "GLStarfield.hxx"
#include "CSector.hxx"

enum class DeathCause : unsigned int
{
	OTHER,
	GUNFIRE,
	MISSILE
};

class CInSpaceState : public IGameState
{
public:
	CInSpaceState(char const *, SectorId const, Vector2f const &);
	~CInSpaceState();

	virtual void state_initializing(void) final;
	virtual void state_prerender_tick(sf::View &, sf::RenderWindow &, float const) final;
	virtual void state_render_world_tick(sf::View &, sf::RenderWindow &, float const) final;
	virtual void state_render_world_ui_tick(sf::View &, sf::RenderWindow &, float const) final;
	virtual void state_render_ui_tick(sf::View &, sf::RenderWindow &, float const) final;
	virtual void state_postrender_tick(sf::RenderWindow &, float const) final;

	virtual void state_terminating(void) final;
	virtual void state_process_event(sf::View &, sf::RenderWindow &, sf::Event &) final;

	virtual bool state_terminated(void) final;

	virtual void state_enable_input(bool const) final;

	virtual void state_send_notification(std::string const &) final;

	void build_death_string(InstanceId const, DeathCause const);

	void set_waypoint(Vector2f const &);
	void remove_waypoint(void);

	void set_rmsn_script(std::string const &);

private:
	void do_targeting_ui(sf::RenderWindow &);
	void update_view(sf::View &);
	void create_ingame_windows(void);
	void create_ingame_menu(void);
	void create_inventory_viewer(void);
	void create_material_viewer(void);
	void create_character_sheet(void);
	void create_ability_repertoire(void);
	void create_skill_tree_panel(void);
	void create_sector_map_panel(void);
	void create_debug_console_panel(void);
	void pull_configuration_settings(void);

	/*
	* LOCK m_mFieldAccess BEFORE
	* ACCESSING
	*/
	bool m_bInputEnabled;

	bool m_bRmsnEnabledForThisSession;
	std::string m_szRmsnScript;

	Vector2f m_vStartingPosition;

	unsigned int m_uiScreenWidth;
	unsigned int m_uiScreenHeight;

	unsigned int m_uiGraphicsQuality;
	unsigned int m_uiControlsStyle;

	bool m_bIngameMenuOpen;

	/*
	 * LOCK m_mFieldAccess BEFORE
	 * ACCESSING
	 */
	bool m_bWaypointPlaced;

	/*
	* LOCK m_mFieldAccess BEFORE
	* ACCESSING
	*/
	Vector2f m_vWaypointPosition;

	Spinlock m_mFieldAccess;
	std::string m_szStartingScript;

	IEngine * m_pEngine;
	IRenderPipeline * m_pRenderPipeline;
	IWorld * m_pWorld;
	IScriptEngine *m_pScriptEngine;
	CGameDataManager *m_pGameDataManager;
	InterfaceManager *m_pInterfaceManager;
	CAudioManager *m_pAudioManager;
	CParticleManager *m_pParticleManager;
	CCommsManager *m_pCommsManager;
	IStaticTextureManager *m_pStaticTextureManager;
	IUniverse *m_pUniverse;

	ICamera * m_pActiveCamera;

	CShip * m_pPlayer;
	std::string m_szPlayerName;
	InstanceId m_iPlayerId;

	sf::VertexArray m_sfBgVerts;
	sf::Shader m_sfBackgroundShader;

	sf::Texture m_sfHudTargetingSelectorTexture;
	sf::Sprite m_sfHudTargetingSelector;
	sf::Texture m_sfHudTargetingReticleTexture;
	sf::Sprite m_sfHudTargetingReticle;
	sf::Texture m_sfHudPointerTexture;
	sf::Sprite m_sfHudPointer;
	sf::Texture m_sfHudTargetingAidTexture;
	sf::Sprite m_sfHudTargetingAid;
	sf::Texture m_sfWaypointTexture;
	sf::Sprite m_sfWaypoint;
	bool m_bRenderTargetingAssistance;

	GLStarfield m_glstarfield;

	InstanceId m_iHUDTarget;

	std::string m_szLastNotificationReceived;
	float m_flTimeSinceLastNotification;
	float m_flDeathScreenTimer;
	std::string m_szDeathString;
	bool m_flDeathScreenSpawned;
	bool m_bTrackMode;
	SectorId m_uiSectorId;
};