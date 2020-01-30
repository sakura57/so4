#include "CInSpaceState.hxx"
#include "CChaseCamera.hxx"
#include "LoadoutPanel.hxx"
#include "RadarPanel.hxx"
#include "ShipStatusPanel.hxx"
#include "TargetPanel.hxx"
#include "InGameMenuPanel.hxx"
#include "CAIController.hxx"
#include "DebugPanel.hxx"
#include "CSectorJumpBuoy.hxx"
#include "NotificationPanel.hxx"
#include "AbilityToolbar.hxx"
#include "AbilityRepertoirePanel.hxx"
#include "SectorMapPanel.hxx"

#include "CEntityInventory.hxx"
#include "InventoryViewerPanel.hxx"
#include "CharacterSheetPanel.hxx"
#include "IIntransientDataManager.hxx"
#include "MaterialBankPanel.hxx"
//#include "CStatusEngineOffline.hxx"
#include "PlayerDiedPanel.hxx"
#include "SkillTreePanel.hxx"
#include "CAsteroidMineable.hxx"
#include "DebugConsolePanel.hxx"

#include <sstream>
#include <gl/GLU.h>

CInSpaceState::CInSpaceState(char const *startingScript, SectorId sectorId, Vector2f const &vStartingPosition)
	: m_glstarfield(8 * DEFAULT_WINDOW_WIDTH, 14 * DEFAULT_WINDOW_HEIGHT),
	m_sfBgVerts(sf::TrianglesStrip, 5),
	m_szStartingScript(startingScript),
	m_bIngameMenuOpen(false),
	m_bWaypointPlaced(false),
	m_vStartingPosition(vStartingPosition),
	m_bInputEnabled(true),
	m_flDeathScreenTimer(1.5f),
	m_flDeathScreenSpawned(false),
	m_szPlayerName(SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity()->get_name()),
	m_bRmsnEnabledForThisSession(false),
	m_szRmsnScript(""),
	m_uiSectorId(sectorId),
	m_bGamePaused(false)
{
	this->m_pEngine = SG::get_engine();
	this->m_pRenderPipeline = SG::get_render_pipeline();
	this->m_pWorld = SG::get_world();
	this->m_pScriptEngine = SG::get_script_engine();
	this->m_pGameDataManager = SG::get_game_data_manager();
	this->m_pInterfaceManager = SG::get_interface_manager();
	this->m_pAudioManager = SG::get_audio_manager();
	this->m_pParticleManager = SG::get_particle_manager();
	this->m_pCommsManager = SG::get_comms_manager();
	this->m_pStaticTextureManager = SG::get_static_texture_manager();
	this->m_pUniverse = SG::get_universe();

	this->pull_configuration_settings();
}

CInSpaceState::~CInSpaceState()
{
}

void CInSpaceState::state_enable_input(bool const bEnableInput)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_bInputEnabled = bEnableInput;

	if(!bEnableInput)
	{
		this->m_pInterfaceManager->free_all_panels();

		//This will cancel pending comms and
		//terminate the current comm.
		this->m_pCommsManager->shifting_out();
	}
	else
	{
		this->create_ingame_windows();
	}
}

void CInSpaceState::state_initializing(void)
{
	/*
	 * Every time we transition to space state,
	 * there is a chance resource pool will be refreshed
	 * and populated with new randomized resources.
	 * We just need to tick the resource manager with the
	 * current absolute delta, he will handle the rest.
	 *
	 * This MUST be done early, at the very least before
	 * the system script executes and potentially attempts
	 * to spawn asteroids which will need fresh mats.
	 */
	SG::get_material_manager()->do_possible_generation_tick(SG::get_game_clock()->get_absolute_delta());

	this->m_flTimeSinceLastNotification = 60.0f;

	/*
	* load the HUD elements (these will be hardcoded)
	*/
	this->m_sfHudTargetingSelectorTexture.loadFromFile(CGameDataManager::get_full_data_file_path("graphics\\targeting_selector.png").c_str());
	this->m_sfHudTargetingSelector.setTexture(this->m_sfHudTargetingSelectorTexture);
	this->m_sfHudTargetingSelector.setScale(0.5f, 0.5f);
	this->m_sfHudTargetingSelector.setOrigin(162.0f, 162.0f);
	this->m_sfHudTargetingReticleTexture.loadFromFile(CGameDataManager::get_full_data_file_path("graphics\\targeting_aid.png").c_str());
	this->m_sfHudTargetingReticle.setTexture(this->m_sfHudTargetingReticleTexture);
	this->m_sfHudTargetingReticle.setScale(0.5f, 0.5f);
	this->m_sfHudTargetingReticle.setOrigin(32.0f, 32.0f);
	this->m_sfHudTargetingAidTexture.loadFromFile(CGameDataManager::get_full_data_file_path("graphics\\targeting_reticle.png").c_str());
	this->m_sfHudTargetingAid.setTexture(this->m_sfHudTargetingAidTexture);
	this->m_sfHudTargetingAid.setScale(0.5f, 0.5f);
	this->m_sfHudTargetingAid.setOrigin(32.0f, 32.0f);
	this->m_sfHudPointerTexture.loadFromFile(CGameDataManager::get_full_data_file_path("graphics\\pointer.png").c_str());
	this->m_sfHudPointer.setTexture(this->m_sfHudPointerTexture);
	this->m_sfHudPointer.setScale(0.5f, 0.5f);
	this->m_sfHudPointer.setOrigin(64.0f, 64.0f);
	this->m_sfWaypointTexture.loadFromFile(CGameDataManager::get_full_data_file_path("graphics\\waypoint.png").c_str());
	this->m_sfWaypoint.setTexture(this->m_sfWaypointTexture);
	this->m_sfWaypoint.setScale(0.5f, 0.5f);
	this->m_sfWaypoint.setOrigin(128.0f, 128.0f);

	this->m_sfBackgroundShader.loadFromFile(CGameDataManager::get_full_data_file_path("shaders\\background.txt").c_str(), sf::Shader::Fragment);
	this->m_sfBgVerts[0].position = sf::Vector2f(DEFAULT_WINDOW_WIDTH, 0);
	this->m_sfBgVerts[1].position = sf::Vector2f(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	this->m_sfBgVerts[2].position = sf::Vector2f(0, DEFAULT_WINDOW_HEIGHT);
	this->m_sfBgVerts[3].position = sf::Vector2f(0, 0);
	this->m_sfBgVerts[4].position = sf::Vector2f(DEFAULT_WINDOW_WIDTH, 0);

	this->m_sfBackgroundShader.setUniform("resolution", sf::Glsl::Vec2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT));

	ICharacterEntity * pPlayerEntity = SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity();

	CChaseCamera *cc;
	InstanceId ccid;
	CInstanceFactory::create<CChaseCamera>(ccid, cc);
	this->m_pWorld->instance_add(cc);
	this->m_pActiveCamera = cc;
	this->m_pRenderPipeline->set_active_camera(cc);

	CShip *obj;
	CInstanceFactory::create<CShip>(this->m_iPlayerId, obj);

	CShip::CreationParameters parms;
	parms.vPosition = this->m_vStartingPosition;
	parms.vVelocity = Vector2f(0.0f, 0.0f);
	parms.flRotation = 0.0f;
	parms.flAngularVelocity = 0.0f;
	parms.pArch = this->m_pGameDataManager->get_arch<ShipArch>(pPlayerEntity->get_ship());
	parms.uiAppearanceId = parms.pArch->uiShipAppearance;
	parms.flHitPoints = parms.pArch->flMaxHitPoints;
	parms.flRadius = 64.0f;
	parms.flHitPoints = parms.pArch->flMaxHitPoints;
	parms.flShieldPoints = parms.pArch->flMaxShieldPoints;
	parms.szName = "Player";
	parms.pFaction = nullptr;
	parms.pPilot = nullptr;
	parms.bActivated = true;
	parms.szRadarClass = "Ship";
	parms.bIsPlayer = true;
	parms.uiInvulnerable = 0U;
	parms.flCapacitorEnergy = parms.pArch->flCapacitorEnergy;
	parms.flCapacitorRegenRate = parms.pArch->flCapacitorRegenRate;
	parms.ulExpValue = 0UL;
	parms.pParentEntity = SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity();
	parms.pStartingLoadout = parms.pParentEntity->get_loadout();
	obj->initialize(parms);
	obj->listener_add(cc);

	//TESTING CODE
	//obj->inflict_status(new CStatusEngineOffline(15.0f));

	cc->acquire_target(obj);
	cc->alive_set(true);
	cc->set_position(this->m_vStartingPosition);
	cc->set_bounds(Vector2f(2.275f * DEFAULT_WINDOW_WIDTH, 2.275f * DEFAULT_WINDOW_HEIGHT));

	this->m_pPlayer = obj;
	this->m_pPlayer->instance_acquired();

	this->m_pWorld->instance_add(obj);
	this->m_pWorld->set_player(this->m_iPlayerId);

	this->create_ingame_windows();

	//Hyperspace exit effect
	SG::get_audio_manager()->play_sound(12);
	SG::get_particle_manager()->add_particle(10, this->m_pPlayer->get_position(), Vector2f(0.0f, 0.0f), 0.0f, 0.0f);

	this->m_pScriptEngine->script_enqueue(this->m_szStartingScript.c_str());

	//this->m_pScriptEngine->script_enqueue("scripts\\stresstest.lua");

	if (this->m_bRmsnEnabledForThisSession)
	{
		this->m_pScriptEngine->script_enqueue(this->m_szRmsnScript.c_str());
	}
}

void CInSpaceState::state_prerender_tick(sf::View &mainView, sf::RenderWindow &sfWindow, float const flDelta)
{
	//update the time since last notification
	this->m_mFieldAccess.lock();
	this->m_flTimeSinceLastNotification += flDelta;
	this->m_mFieldAccess.unlock();

	if(this->m_pPlayer == nullptr && !this->m_flDeathScreenSpawned)
	{
		if(this->m_flDeathScreenTimer > 0.0f)
		{
			this->m_flDeathScreenTimer -= flDelta;
		}
		else if(this->m_flDeathScreenTimer <= 0.0f)
		{
			PlayerDiedPanel *pDiedPanel = new PlayerDiedPanel(this->m_szDeathString);
			this->m_pInterfaceManager->add_panel(pDiedPanel);

			this->m_flDeathScreenSpawned = true;
		}
	}

	if(this->m_pPlayer && !this->m_pPlayer->is_alive())
	{
		this->m_pPlayer->instance_released();
		this->m_pPlayer = nullptr;
		return;
	}

	if(this->m_pPlayer)
	{
		this->m_pAudioManager->set_listener_point(this->m_pPlayer->get_position());
	}

	this->m_mFieldAccess.lock();
	volatile bool bContinue = this->m_bInputEnabled;
	this->m_mFieldAccess.unlock();

	if(!bContinue)
	{
		return;
	}

	//BEGIN TESTING CODE
	if (this->m_pPlayer)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(sfWindow);

		sf::Vector2f relativeWorldPos = sfWindow.mapPixelToCoords(mousePos, mainView);

		if(!this->m_pInterfaceManager->input_blocked())
		{
			if(this->m_uiControlsStyle == 1)
			{
				if(this->m_bTrackMode)
				{
					CAIController::aim_at_point(this->m_pPlayer, Vector2f(relativeWorldPos.x, relativeWorldPos.y));
				}
				else
				{
					this->m_pPlayer->set_spin_throttle(0.0f);
				}

				if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					this->m_pPlayer->fire_weapons_if_ready(ARCH_WEAPON);
				}

				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
				{
					this->m_pPlayer->set_throttle(1.0f);
				}
				else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
				{
					this->m_pPlayer->set_throttle(-1.0f);
				}
				else
				{
					this->m_pPlayer->set_throttle(0.0f);
				}

				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
				{
					this->m_pPlayer->set_strafe_throttle(-1.0f);
				}
				else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
				{
					this->m_pPlayer->set_strafe_throttle(1.0f);
				}
				else
				{
					this->m_pPlayer->set_strafe_throttle(0.0f);
				}
			}
			else
			{
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
				{
					this->m_pPlayer->fire_weapons_if_ready(ARCH_WEAPON);
				}

				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
				{
					this->m_pPlayer->set_throttle(1.0f);
				}
				else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
				{
					this->m_pPlayer->set_throttle(-1.0f);
				}
				else
				{
					this->m_pPlayer->set_throttle(0.0f);
				}

				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
				{
					this->m_pPlayer->set_spin_throttle(1.0f);
				}
				else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
				{
					this->m_pPlayer->set_spin_throttle(-1.0f);
				}
				else
				{
					this->m_pPlayer->set_spin_throttle(0.0f);
				}
			}
		}
	}

	//END TESTING CODE
}

void CInSpaceState::state_render_world_tick(sf::View &sfView, sf::RenderWindow &sfWindow, float const flDelta)
{
	if(this->m_uiGraphicsQuality > 2)
	{
		sf::RenderStates states;
		states.shader = &this->m_sfBackgroundShader;

		sf::Vector2f vCameraPosition = sfView.getCenter();

		this->m_sfBackgroundShader.setUniform("offset", sf::Glsl::Vec2(vCameraPosition.x * 0.00025f, vCameraPosition.y * 0.00025f));

		sfWindow.draw(this->m_sfBgVerts, states);
	}

	this->update_view(sfView);
	sfWindow.setView(sfView);

	//render static world objects

	sfWindow.pushGLStates();
	sfWindow.setActive(true);

	//update the GL state
	{
		sf::FloatRect viewport = sfView.getViewport();
		
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.01f);

		glDisable(GL_LIGHTING);

		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();

		gluPerspective(70.0f, (float)DEFAULT_WINDOW_WIDTH / DEFAULT_WINDOW_HEIGHT, 0.1f, 10000.0f);

		sf::Vector2f vCameraPosition = sfView.getCenter();

		gluLookAt(vCameraPosition.x, vCameraPosition.y, 0.0f,
			vCameraPosition.x, vCameraPosition.y, -1.0f,
			0.0f, 1.0f, 0.0f);
	}

	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

	if (this->m_uiGraphicsQuality < 3)
	{
		//TODO: I wrote GLStarfield a long ass time ago, for a different project,
		//it's a hunk of junk especially with larger resolutions it wasn't intended for.
		//Rewrite it or implement a new LQ background, until then, enjoy the blackness of space.

		//this->m_glstarfield.update_starfield(Vector2f(sfView.getCenter().x, sfView.getCenter().y));
		//this->m_glstarfield.draw_starfield(&sfWindow);
	}

	//Render staticobjs
	{
		/*
		 * Acquire the list of statics.
		 * We must release by calling release_statics() at the end of the current block
		 */
		std::vector<CStaticObject> const &vStaticObjs = this->m_pWorld->get_all_statics();

		for(CStaticObject const &staticObj : vStaticObjs)
		{
			StaticTextureRenderParameters renderParms;
			Vector2f vBounds;

			staticObj.get_position(renderParms.flPositionX, renderParms.flPositionY, renderParms.flPositionZ);
			staticObj.get_rotation(renderParms.flRotation);
			staticObj.get_bounds(vBounds);

			renderParms.flHeight = vBounds.x;
			renderParms.flWidth = vBounds.y;

			this->m_pStaticTextureManager->render_texture(staticObj.get_texture_id(), renderParms);
		}

		this->m_pWorld->release_statics();
	}

	sfWindow.setActive(false);
	sfWindow.popGLStates();
}

void CInSpaceState::state_render_world_ui_tick(sf::View &sfView, sf::RenderWindow &sfWindow, float const flDelta)
{
	//render UI
	this->do_targeting_ui(sfWindow);
}

void CInSpaceState::state_render_ui_tick(sf::View &, sf::RenderWindow &, float const)
{
}

void CInSpaceState::state_postrender_tick(sf::RenderWindow &, float const)
{
}

void CInSpaceState::state_terminating(void)
{
	if(this->m_pActiveCamera != nullptr)
	{
		if(this->m_pActiveCamera->instance_get_flags() & CChaseCamera::InstanceFlag)
		{
			CChaseCamera* pChaseCamera = static_cast<CChaseCamera*>(this->m_pActiveCamera);
			pChaseCamera->release_target();
		}

		this->m_pActiveCamera = nullptr;
	}

	this->m_pPlayer = nullptr;
}

void CInSpaceState::state_process_event(sf::View &mainView, sf::RenderWindow &sfWindow, sf::Event &sfEvent)
{
	this->m_mFieldAccess.lock();
	volatile bool bContinue = this->m_bInputEnabled;
	this->m_mFieldAccess.unlock();

	if(!bContinue)
	{
		return;
	}

	if(!this->m_pInterfaceManager->input_blocked())
	{
		if(sfEvent.type == sf::Event::MouseButtonPressed)
		{
			if(this->m_pPlayer)
			{
				/*
				 * We detected a left mouse down event. Convert to world coordinates and pass these to the
				 * world to see if there is an instance where we clicked.
				 */
				if(sfEvent.mouseButton.button == sf::Mouse::Left && !ImGui::IsMouseHoveringAnyWindow())
				{
					sf::Vector2f relativeWorldPos =
						sfWindow.mapPixelToCoords(
							sf::Vector2i(sfEvent.mouseButton.x, sfEvent.mouseButton.y),
							mainView);

					CWorldTransaction wTransaction;

					InstanceId targetId = this->m_pWorld->get_instance_at(Vector2f(relativeWorldPos.x, relativeWorldPos.y));

					if(targetId != INVALID_INSTANCE && targetId != this->m_pPlayer->instance_get_id())
					{
						IWorldInstance* pInstance = this->m_pEngine->instance_get(targetId);

						if(pInstance->instance_get_flags() & CEquippedObject::InstanceFlag)
						{
							this->m_pPlayer->set_target(targetId);

							this->m_pAudioManager->play_sound(30U);
						}
					}
					else
					{
						this->m_pPlayer->set_target(INVALID_INSTANCE);
					}
				}
			}
		}
		else if(sfEvent.type == sf::Event::KeyPressed)
		{
			if(this->m_pPlayer)
			{
				for(int i = 1; i <= 10; ++i)
				{
					SpellId iSpellSlot = i % 10;

					if(sfEvent.key.code == sf::Keyboard::Key::Num0 + iSpellSlot)
					{
						ICharacterEntity* pPlayerEntity = SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity();

						//UI click sound effect
						this->m_pInterfaceManager->register_tangible_ui_event();

						if(pPlayerEntity->can_cast_spell(iSpellSlot))
						{
							pPlayerEntity->cast_spell(iSpellSlot, this->m_pPlayer->instance_get_id(), this->m_pPlayer->get_target());
						}
					}
				}

				if(sfEvent.key.code == sf::Keyboard::Space)
				{
					this->m_pPlayer->fire_weapons_if_ready(ARCH_MISSILE);
				}
				else if(sfEvent.key.code == sf::Keyboard::T)
				{
					InstanceId targetId = this->m_pPlayer->get_target();

					this->m_pInterfaceManager->register_tangible_ui_event();

					if(targetId != INVALID_INSTANCE)
					{
						CWorldTransaction wTransaction;

						IWorldInstance* pInstance = this->m_pEngine->instance_get(targetId);

						if(pInstance->instance_get_flags() & CEquippedObject::InstanceFlag)
						{
							CEquippedObject* pEqObj = SG::get_engine()->instance_get_checked<CEquippedObject>(targetId);

							pEqObj->interact(this->m_pPlayer->instance_get_id());
						}
					}
				}
				else if(sfEvent.key.code == sf::Keyboard::I)
				{
					this->m_pInterfaceManager->register_tangible_ui_event();

					this->create_inventory_viewer();
				}
				else if(sfEvent.key.code == sf::Keyboard::U)
				{
					this->m_pInterfaceManager->register_tangible_ui_event();

					this->create_character_sheet();
				}
				else if(sfEvent.key.code == sf::Keyboard::Y)
				{
					this->m_pInterfaceManager->register_tangible_ui_event();

					this->create_ability_repertoire();
				}
				else if(sfEvent.key.code == sf::Keyboard::G)
				{
					this->m_pInterfaceManager->register_tangible_ui_event();

					this->create_skill_tree_panel();
				}
				else if(sfEvent.key.code == sf::Keyboard::K)
				{
					this->m_pInterfaceManager->register_tangible_ui_event();

					this->create_material_viewer();
				}
				else if(sfEvent.key.code == sf::Keyboard::M)
				{
					this->m_pInterfaceManager->register_tangible_ui_event();

					this->create_sector_map_panel();
				}
				else if(sfEvent.key.code == sf::Keyboard::LControl)
				{
					this->m_bTrackMode = !this->m_bTrackMode;

					this->state_send_notification(std::string("Mouse Track: ") + std::string(this->m_bTrackMode ? "ON" : "OFF"));
				}
				else if(sfEvent.key.code == sf::Keyboard::Tilde)
				{
					this->m_pInterfaceManager->register_tangible_ui_event();

					this->create_debug_console_panel();
				}
			}

			if(sfEvent.key.code == sf::Keyboard::Escape)
			{
				this->m_pInterfaceManager->register_tangible_ui_event();

				if(this->m_bIngameMenuOpen)
				{
					this->create_ingame_windows();
				}
				else
				{
					this->create_ingame_menu();
				}

				this->m_bIngameMenuOpen = !this->m_bIngameMenuOpen;
			}
		}
	}
}

bool CInSpaceState::state_terminated(void)
{
	return false;
}

void CInSpaceState::set_waypoint(Vector2f const &vWayPosition)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vWaypointPosition = vWayPosition;
	this->m_bWaypointPlaced = true;
}

void CInSpaceState::remove_waypoint(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_bWaypointPlaced = false;
}

/*
* Render the targeting UI. Should be called where UI is rendered
* in the rendering loop.
*
* This includes the targeting selector, and aiming reticle if it is
* enabled.
*/
void CInSpaceState::do_targeting_ui(sf::RenderWindow &sfWindow)
{
	if (this->m_pPlayer == nullptr || !this->m_pPlayer->is_alive())
	{
		return;
	}

	if(this->m_pActiveCamera == nullptr)
	{
		return;
	}

	if(this->m_bInputEnabled == false)
	{
		return;
	}

	this->m_iHUDTarget = this->m_pPlayer->get_target();

	//TODO: might not want to always do this
	this->m_bRenderTargetingAssistance = true;

	//render the waypoint
	this->m_mFieldAccess.lock();
	if(this->m_bWaypointPlaced)
	{
		Vector2f vTargetPosition = this->m_vWaypointPosition;
		this->m_mFieldAccess.unlock();

		Vector2f vCameraPosition = this->m_pActiveCamera->get_position();
		Vector2f vDelta = vTargetPosition - vCameraPosition;

		sf::Color selectorColor(0.0f, 232.0f, 255.0f, 232.0f);

		float flWidthLimit = (DEFAULT_WINDOW_WIDTH - (DEFAULT_WINDOW_WIDTH / 12.0f));
		float flHeightLimit = (DEFAULT_WINDOW_HEIGHT - (DEFAULT_WINDOW_HEIGHT / 12.0f));

		//if the target is within range, display the waypoint.
		// otherwise, render the pointer
		//if(vDelta.magnitude() < 850.0f)
		if (abs(vDelta.x) < flWidthLimit
			&& abs(vDelta.y) < flHeightLimit)
		{
			//render the waypoint
			this->m_sfWaypoint.setPosition(vTargetPosition.x, vTargetPosition.y);
			this->m_sfWaypoint.setColor(selectorColor);
			sfWindow.draw(this->m_sfWaypoint);
		}
		else
		{
			Vector2f vNormalizedDelta = vDelta.normalize();
			float flAngle = rad2deg(atan2f(vNormalizedDelta.y, vNormalizedDelta.x));
			//Vector2f vCircleOffset = flHeightLimit * vNormalizedDelta;
			Vector2f vCircleOffset(vNormalizedDelta.x * flWidthLimit, vNormalizedDelta.y * flHeightLimit);
			Vector2f vRenderArrowPosition = vCircleOffset + vCameraPosition;

			this->m_sfHudPointer.setPosition(vRenderArrowPosition.x, vRenderArrowPosition.y);
			this->m_sfHudPointer.setRotation(flAngle);
			this->m_sfHudPointer.setColor(selectorColor);
			sfWindow.draw(this->m_sfHudPointer);
		}
	}
	else
	{
		this->m_mFieldAccess.unlock();
	}

	if (this->m_iHUDTarget > 0)
	{
		this->m_pWorld->begin_world_transaction();

		if (this->m_pEngine->instance_is_allocated(this->m_iHUDTarget))
		{
			IWorldInstance *pTargetInstance = this->m_pEngine->instance_get(this->m_iHUDTarget);

			if (pTargetInstance->instance_get_flags() & IWorldObject::InstanceFlag)
			{
				IWorldObject *pTargetObject = static_cast<IWorldObject*>(pTargetInstance);

				Vector2f vTargetPosition = pTargetObject->get_position();
				Vector2f vCameraPosition = this->m_pActiveCamera->get_position();
				Vector2f vDelta = vTargetPosition - vCameraPosition;

				sf::Color selectorColor(255.0f, 255.0f, 255.0f, 232.0f);

				if (pTargetObject->instance_get_flags() & CEquippedObject::InstanceFlag)
				{
					CEquippedObject *pEqObj = static_cast<CEquippedObject*>(pTargetObject);

					Attitude attitude = pEqObj->get_attitude_set().get_attitude(this->m_iPlayerId);

					if (attitude <= THRESHOLD_HOSTILE)
					{
						selectorColor = sf::Color(255.0f, 0.0f, 0.0f, 232.0f);
					}
					else if (attitude >= THRESHOLD_FRIENDLY)
					{
						selectorColor = sf::Color(0.0f, 255.0f, 0.0f, 232.0f);
					}
				}

				float flWidthLimit = (DEFAULT_WINDOW_WIDTH - (DEFAULT_WINDOW_WIDTH / 12.0f));
				float flHeightLimit = (DEFAULT_WINDOW_HEIGHT - (DEFAULT_WINDOW_HEIGHT / 12.0f));

				//if the target is within range, display the targeting
				//selector. otherwise, render the pointer
				//if(vDelta.magnitude() < 850.0f)
				if (abs(vDelta.x) < flWidthLimit
					&& abs(vDelta.y) < flHeightLimit)
				{
					//render the targeting selector
					this->m_sfHudTargetingSelector.setPosition(vTargetPosition.x, vTargetPosition.y);
					this->m_sfHudTargetingSelector.setColor(selectorColor);
					sfWindow.draw(this->m_sfHudTargetingSelector);
				}
				else
				{
					Vector2f vNormalizedDelta = vDelta.normalize();
					float flAngle = rad2deg(atan2f(vNormalizedDelta.y, vNormalizedDelta.x));
					//Vector2f vCircleOffset = flHeightLimit * vNormalizedDelta;
					Vector2f vCircleOffset(vNormalizedDelta.x * flWidthLimit, vNormalizedDelta.y * flHeightLimit);
					Vector2f vRenderArrowPosition = vCircleOffset + vCameraPosition;

					this->m_sfHudPointer.setPosition(vRenderArrowPosition.x, vRenderArrowPosition.y);
					this->m_sfHudPointer.setRotation(flAngle);
					this->m_sfHudPointer.setColor(selectorColor);
					sfWindow.draw(this->m_sfHudPointer);
				}

				//render the targeting reticle
				if (this->m_bRenderTargetingAssistance)
				{
					Vector2f vPlayerPosition = this->m_pPlayer->get_position();
					//Vector2f vTargetPosition = pTargetObject->get_position();
					Vector2f vPlayerVelocity = this->m_pPlayer->get_velocity();
					float flPlayerRotation = this->m_pPlayer->get_rotation();

					float flAverageMuzzleVelocity = this->m_pPlayer->get_average_weapon_muzzle_velocity();
					Vector2f vPositionDelta = vTargetPosition - vPlayerPosition;
					float flTravelDistance = vPositionDelta.magnitude();
					float flAverageTravelTime = flTravelDistance / flAverageMuzzleVelocity;

					Vector2f vDecidedPosition;

					if (pTargetObject->instance_get_flags() & IPhysicalObject::InstanceFlag)
					{
						IPhysicalObject *pPhysicalObject = pTargetObject->get_physical();

						if (pPhysicalObject)
						{
							Vector2f vTargetVelocity = pPhysicalObject->get_velocity();

							Vector2f vPredictedTargetPosition = vTargetPosition + (flAverageTravelTime * vTargetVelocity) - (flAverageTravelTime * vPlayerVelocity);
							vDecidedPosition = vPredictedTargetPosition;

							this->m_sfHudTargetingReticle.setPosition(vPredictedTargetPosition.x, vPredictedTargetPosition.y);
						}

						sfWindow.draw(this->m_sfHudTargetingReticle);
					}
					else
					{
						//if the target has no physics interface, and therefore no velocity,
						//render the reticle directly on the object
						vDecidedPosition = vTargetPosition;

						this->m_sfHudTargetingReticle.setPosition(vTargetPosition.x, vTargetPosition.y);
					}

					//draw the targeting aid, which shows the eventual position
					//of shots fired by the player
					Vector2f vNeededPositionDelta = vDecidedPosition - vPlayerPosition;
					float flNeededTravelDistance = vNeededPositionDelta.magnitude();
					float flNeededAverageTravelTime = flNeededTravelDistance / flAverageMuzzleVelocity;

					Vector2f vTravelVector = (Matrix2f::rotation_matrix(flPlayerRotation) * Vector2f(flNeededTravelDistance, 0.0f));

					Vector2f vShotLandingPosition = vPlayerPosition + vTravelVector;

					this->m_sfHudTargetingAid.setPosition(vShotLandingPosition.x, vShotLandingPosition.y);

					sfWindow.draw(this->m_sfHudTargetingAid);
				}
			}
		}
		else
		{
			//the target has obviously been destroyed if the engine
			//has it marked as unallocated, so we will reset our target
			//in this case

			this->m_iHUDTarget = -1;
		}

		this->m_pWorld->end_world_transaction();
	}
}

/*
* This function acquires a handle to the active camera from the render pipeline,
* then updates an sf::View class with the camera's information.
*/
void CInSpaceState::update_view(sf::View &sfView)
{
	ICamera *pCamera = this->m_pRenderPipeline->acquire_active_camera();

	if (pCamera == nullptr)
	{
		//if there is no active camera, do not call release_active_camera(),
		//just return and don't update anything
		return;
	}

	Vector2f vBounds = pCamera->get_bounds();
	Vector2f vPosition = pCamera->get_position();

	//there should be no usage of pCamera beyond release_active_camera()
	this->m_pRenderPipeline->release_active_camera();

	sfView.setCenter(vPosition.x, vPosition.y);
	sfView.setSize(vBounds.x, -vBounds.y);
}

void CInSpaceState::create_ingame_windows(void)
{
	this->m_pInterfaceManager->free_all_panels();

	ICharacterEntityManager *pCEM = SG::get_intransient_data_manager()->get_character_entity_manager();

	RadarPanel *pRadarPanel = new RadarPanel(this->m_iPlayerId);
	this->m_pInterfaceManager->add_panel(pRadarPanel);
	LoadoutPanel *pLoadoutPanel = new LoadoutPanel(this->m_iPlayerId);
	this->m_pInterfaceManager->add_panel(pLoadoutPanel);
	ShipStatusPanel *pStatusPanel = new ShipStatusPanel(this->m_iPlayerId);
	this->m_pInterfaceManager->add_panel(pStatusPanel);
	TargetPanel *pTargetPanel = new TargetPanel(this->m_iPlayerId);
	this->m_pInterfaceManager->add_panel(pTargetPanel);
	DebugPanel *pDebugPanel = new DebugPanel();
	this->m_pInterfaceManager->add_panel(pDebugPanel);
	//NotificationPanel *pNotificationPanel = new NotificationPanel();
	//this->m_pInterfaceManager->add_panel(pNotificationPanel);
	AbilityToolbar *pAbilityToolbar = new AbilityToolbar(this->m_iPlayerId, pCEM->get_player_character_entity());
	this->m_pInterfaceManager->add_panel(pAbilityToolbar);
}

void CInSpaceState::create_ingame_menu(void)
{
	this->m_pInterfaceManager->free_all_panels();

	this->m_pInterfaceManager->add_panel(new InGameMenuPanel);
}

void CInSpaceState::create_inventory_viewer(void)
{
	IIntransientDataManager *pIDM = SG::get_intransient_data_manager();
	IEntityInventory *pInventory = pIDM->get_character_entity_manager()->get_player_character_entity()->get_inventory();

	this->m_pInterfaceManager->add_panel(new InventoryViewerPanel(pInventory));
}

void CInSpaceState::create_character_sheet(void)
{
	IIntransientDataManager *pIDM = SG::get_intransient_data_manager();
	ICharacterEntity *pPlayerEntity = pIDM->get_character_entity_manager()->get_player_character_entity();

	this->m_pInterfaceManager->add_panel(new CharacterSheetPanel(pPlayerEntity));
}

void CInSpaceState::create_skill_tree_panel(void)
{
	IIntransientDataManager* pIDM = SG::get_intransient_data_manager();
	ICharacterEntity* pPlayerEntity = pIDM->get_character_entity_manager()->get_player_character_entity();

	this->m_pInterfaceManager->add_panel(new SkillTreePanel(pPlayerEntity));
}

void CInSpaceState::create_ability_repertoire(void)
{
	IIntransientDataManager *pIDM = SG::get_intransient_data_manager();
	ICharacterEntity *pPlayerEntity = pIDM->get_character_entity_manager()->get_player_character_entity();

	this->m_pInterfaceManager->add_panel(new AbilityRepertoirePanel(pPlayerEntity));
}

void CInSpaceState::create_sector_map_panel(void)
{
	this->m_pInterfaceManager->add_panel(new SectorMapPanel(this->m_uiSectorId));
}

void CInSpaceState::create_debug_console_panel(void)
{
	this->m_pInterfaceManager->add_panel(new DebugConsolePanel);
}

void CInSpaceState::create_material_viewer(void)
{
	IIntransientDataManager* pIDM = SG::get_intransient_data_manager();
	ICharacterEntity* pPlayerEntity = pIDM->get_character_entity_manager()->get_player_character_entity();
	IMaterialBank* pMatBank = pPlayerEntity->get_material_bank();

	this->m_pInterfaceManager->add_panel(new MaterialBankPanel(pMatBank));
}

void CInSpaceState::pull_configuration_settings(void)
{
	this->m_uiGraphicsQuality = 0;
	SG::get_game_settings()->get_setting<unsigned int>("quality_level", this->m_uiGraphicsQuality);
	this->m_uiControlsStyle = 0;
	SG::get_game_settings()->get_setting<unsigned int>("controls_style", this->m_uiControlsStyle);
}

void CInSpaceState::state_send_notification(std::string const &szNotification)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	//anti-notification spam
	if(this->m_flTimeSinceLastNotification > 1.0f || szNotification != this->m_szLastNotificationReceived)
	{
		this->m_pInterfaceManager->add_panel(new NotificationPanel(szNotification));
		this->m_flTimeSinceLastNotification = 0.0f;
		this->m_szLastNotificationReceived = szNotification;
	}
}

void CInSpaceState::build_death_string(InstanceId const uiKillerId, DeathCause const deathCause)
{
	std::stringstream ss;
	bool bIdentifiedKiller = false;

	if(this->m_pPlayer != nullptr)
	{
		ss << "Just like that, " << this->m_szPlayerName << "\'s " << this->m_pPlayer->get_ship_arch()->szShipShortName << " is ripped apart by an explosion." << std::endl;
		ss << "May they find rest and become one with the stars." << std::endl << std::endl;
	}

	if(this->m_pEngine->instance_is_allocated(uiKillerId))
	{
		IWorldInstance *pKillerInstance = this->m_pEngine->instance_get(uiKillerId);

		if(pKillerInstance->instance_get_flags() & CShip::InstanceFlag)
		{
			CShip *pKillerShip = SG::get_engine()->instance_get_checked<CShip>(uiKillerId);

			Faction const *pKillerFaction = pKillerShip->get_faction();
			ss << "Defeated by " << pKillerShip->get_name();

			if(pKillerFaction != nullptr)
			{
				ss << ", " << pKillerFaction->name;
			}

			ss << "." << std::endl;

			bIdentifiedKiller = true;
		}
		else if(pKillerInstance->instance_get_flags() & CEquippedObject::InstanceFlag)
		{
			CEquippedObject *pKillerEqObj = SG::get_engine()->instance_get_checked<CEquippedObject>(uiKillerId);

			Faction const *pKillerFaction = pKillerEqObj->get_faction();
			ss << "Defeated by " << pKillerEqObj->get_object_name();

			if(pKillerFaction != nullptr)
			{
				ss << ", " << pKillerFaction->name;
			}

			ss << "." << std::endl;

			bIdentifiedKiller = true;
		}
	}

	ss << "Cause of death: ";

	switch(deathCause)
	{
	case DeathCause::GUNFIRE:
		ss << "Weapons fire";
		break;
	case DeathCause::MISSILE:
		ss << "Missile";
		break;
	case DeathCause::OTHER:
		ss << "Unknown";
		break;
	}

	if(bIdentifiedKiller)
	{
		this->m_szDeathString = ss.str();
	}
}

void CInSpaceState::set_rmsn_script(std::string const& szRmsnScript)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_bRmsnEnabledForThisSession = true;
	this->m_szRmsnScript = szRmsnScript;
}

bool CInSpaceState::state_game_paused(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	//TODO: for now, pause the game whenever interface manager reports input block
	return this->m_pInterfaceManager->input_blocked();
}