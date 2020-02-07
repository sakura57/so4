#include <chrono>
#include <thread>
#include <iostream>

#include "SGLib.hxx"
#include "CGame.hxx"
#include "CInstanceFactory.hxx"
#include "CChaseCamera.hxx"
#include "CShip.hxx"
#include "CGameDataManager.hxx"

#include "LoadoutPanel.hxx"
#include "RadarPanel.hxx"
#include "ShipStatusPanel.hxx"
#include "TargetPanel.hxx"
#include "CAIController.hxx"
#include "DebugPanel.hxx"

#include "CMainMenuState.hxx"
#include "Spells.hxx"
#include "DebugCommands.hxx"

//TODO: shouldn't have to include this
#include <gl/GLU.h>

CGame::CGame(void)
	: resX(DEFAULT_WINDOW_WIDTH),
		resY(DEFAULT_WINDOW_HEIGHT),
		m_sfWindow(
		sf::VideoMode(
			resX, resY),
		DEFAULT_WINDOW_NAME,
		sf::Style::None | sf::Style::Close | (DEFAULT_WINDOW_FULLSCREEN ? sf::Style::Fullscreen : sf::Style::None),
		//	sf::Style::None | sf::Style::Close,
		sf::ContextSettings(24, 0, 8, 3, 1, sf::ContextSettings::Attribute::Default, false))
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
	this->m_pGameStateManager = SG::get_game_state_manager();
	this->m_pStaticTextureManager = SG::get_static_texture_manager();
	this->m_pUniverse = SG::get_universe();
	this->m_pLootTableManager = SG::get_loot_table_manager();
	this->m_pMaterialManager = SG::get_material_manager();
	this->m_pGameClock = SG::get_game_clock();
	this->m_pDebugConsole = SG::get_debug_console();
	this->m_pDialogueManager = SG::get_dialogue_manager();

	//populate the debug console command map
	populate_debug_commands();

	//threads will continue until otherwise instructed to stop
	this->m_bMainLoopContinue.store(true);

	//game starts unpaused
	this->m_bGamePaused.store(false);

	//limiting framerate to 60 as opposed to vsync on
	//seems to decrease cpu usage
	this->m_sfWindow.setFramerateLimit(0);
	this->m_sfWindow.setVerticalSyncEnabled(true);
	//this->m_sfWindow.setActive(true);

	//initialize ImGui
	ImGui::SFML::Init(this->m_sfWindow);
}

CGame::~CGame(void)
{
	this->m_sfWindow.close();

	SG::get_game_data_manager()->save_settings_data();
}

/*
 * This function is the primary main loop of the game.
 * 
 * Before the loop is entered, the two delegate threads are
 * spawned, the world thread and the renderer thread.
 * 
 * After the other threads are spawned, this function simply
 * blocks until other threads return.
 */
void CGame::enter_main_loop(void)
{
	this->m_pGameStateManager->notify_initiate_loading();

	this->m_pGameStateManager->transition_game_state(new CMainMenuState);

	std::thread loadingThread(&CGame::load_data_delegate, this);
	loadingThread.detach();

	std::thread worldThread(&CGame::enter_world_loop, this);
	std::thread scriptThread(&CGame::enter_script_loop, this);
	
	this->enter_render_loop();

	scriptThread.join();
	worldThread.join();
}

void CGame::load_data_delegate(void)
{
	std::cout << "LOADING..." << std::endl;

	try
	{
		std::cout << "Loading sprite assets." << std::endl;
		CGameDataManager::load_all_graphics_assets(this->m_pRenderPipeline);
		std::cout << "Loading audio assets." << std::endl;
		CGameDataManager::load_all_audio_assets(this->m_pAudioManager);
		std::cout << "Loading animation data." << std::endl;
		CGameDataManager::load_all_animation_assets(this->m_pParticleManager);
		std::cout << "Loading static images." << std::endl;
		CGameDataManager::load_all_static_graphics_assets(this->m_pStaticTextureManager);
		std::cout << "Loading sector data." << std::endl;
		CGameDataManager::load_all_sectors(this->m_pUniverse);
		std::cout << "Loading base data." << std::endl;
		CGameDataManager::load_all_bases(this->m_pUniverse);
		std::cout << "Loading astfield data." << std::endl;
		CGameDataManager::load_all_astfields(this->m_pUniverse);
		std::cout << "Loading loot tables." << std::endl;
		CGameDataManager::load_all_loottables(this->m_pLootTableManager);
		std::cout << "Loading resource classes." << std::endl;
		CGameDataManager::load_all_resource_classes(this->m_pMaterialManager);
		std::cout << "Loading dialogue." << std::endl;
		CGameDataManager::load_all_dialogue(this->m_pDialogueManager);

		//std::cout << "Generating resources." << std::endl;
		//this->m_pMaterialManager->force_generation();

		std::cout << "Loading shiparch." << std::endl;
		this->m_pGameDataManager->load_shiparch_data();
		std::cout << "Loading equiparch." << std::endl;
		this->m_pGameDataManager->load_equiparch_data();
		std::cout << "Loading loadouts." << std::endl;
		this->m_pGameDataManager->load_loadouts_data();
		std::cout << "Loading faction data." << std::endl;
		this->m_pGameDataManager->load_factions_data();
		std::cout << "Loading character data." << std::endl;
		this->m_pGameDataManager->load_characters_data();
		std::cout << "Loading pilot data." << std::endl;
		this->m_pGameDataManager->load_pilots_data();
		std::cout << "Loading name collections." << std::endl;
		this->m_pGameDataManager->load_namecoll_data();
		std::cout << "Loading skill tree data." << std::endl;
		this->m_pGameDataManager->load_skill_data();

		std::cout << "Initializing characters." << std::endl;
		this->m_pCharEntityManager = SG::get_intransient_data_manager()->get_character_entity_manager();

		std::cout << "Initializing skills." << std::endl;
		add_all_spells(SG::get_spell_manager());
	}
	catch(SGException e)
	{
		this->core_fault(e);

		return;
	}

	std::cout << "Finished loading! -> Main menu transition" << std::endl;

	this->m_pGameStateManager->notify_loading_finished();
}

/*
 * Note: render loop is synced with the framerate.
 * 
 * Render loop also handles input and window events, and drives
 * the sound manager.
 */
void CGame::enter_render_loop(void)
{
	sf::View mainView(sf::Vector2f(0.0f, 0.0f), sf::Vector2f((float)resX, -(float)resY));
	sf::Event sfEvent;

	bool bContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();

	sf::Clock imDeltaClock;

	while(bContinue)
	{
		auto time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> time_diff = time - lastTime;
		float flDelta = time_diff.count();

		//tick the game clock
		this->m_pGameClock->tick(flDelta);

		//do game state code
		if(this->m_pGameStateManager->has_pending_game_state())
		{
			this->m_pGameStateManager->shift_to_pending_state();
		}

		IGameState *pGameState = this->m_pGameStateManager->get_game_state();

		if(pGameState->state_terminated())
		{
			this->m_bMainLoopContinue.store(false);

			bContinue = false;

			break;
		}

		if(pGameState->state_game_paused() && this->m_bGamePaused.load() == false)
		{
			this->m_bGamePaused.store(true);
		}
		else if(!pGameState->state_game_paused() && this->m_bGamePaused.load() == true)
		{
			this->m_bGamePaused.store(false);
		}

		if(this->m_bGamePaused.load() == false)
		{
			//tick character entities (spell cooldowns, etc)
			this->m_pCharEntityManager->tick(flDelta);
		}

		//call do_sounds to clean up finished sounds, as well as
		//play queued sounds
		this->m_pAudioManager->do_sounds();

		//LOCK the window access mutex. This mutex should remain
		//locked for the entire render operation.
		this->m_mWindowAccess.lock();

		//poll for window events, and handle input
		while(this->m_sfWindow.pollEvent(sfEvent))
		{
			ImGui::SFML::ProcessEvent(sfEvent);

			if (sfEvent.type == sf::Event::Closed)
			{
				this->m_bMainLoopContinue.store(false);
			}

			//DO STATE EVENT CALL HERE
			pGameState->state_process_event(mainView, this->m_sfWindow, sfEvent);
		}

		//DO STATE PRE RENDER CALL HERE
		pGameState->state_prerender_tick(mainView, this->m_sfWindow, flDelta);

		//begin the rendering operation by clearing the screen
		this->m_sfWindow.clear(sf::Color::Black);

		if(pGameState->state_render_world())
		{
			//DO STATE WORLD RENDER TICK HERE
			pGameState->state_render_world_tick(mainView, this->m_sfWindow, flDelta);

			this->m_pWorld->begin_world_transaction();
			//we will begin walking world instances; if we encounter a world object,
			//render the object
			IWorldInstance* pInstance = this->m_pWorld->instance_walk_begin();

			if(pInstance == nullptr)
			{
				goto NO_INSTANCES_TO_RENDER;
			}

			do
			{
				InstanceFlags uiFlags = pInstance->instance_get_flags();

				//is the instance a world object?
				if(uiFlags & IWorldObject::InstanceFlag)
				{
					IWorldObject* pObject = static_cast<IWorldObject*>(pInstance);

					RenderParameters renderParms;
					pObject->get_render_parms(renderParms);

					try {
						this->m_pRenderPipeline->render_object(pObject, this->m_sfWindow, renderParms);
					}
					catch(SGException e) {
						this->core_fault(e);
					}
				}

			} while(pInstance = this->m_pWorld->instance_walk_next());

NO_INSTANCES_TO_RENDER: //this label should come immediately after the render loop
			this->m_pWorld->end_world_transaction();

			//render particles (particle updation was moved to the world thread)
			this->m_pParticleManager->do_particles(this->m_sfWindow, flDelta);
		}
		
		//RENDER STATE WORLD UI HERE
		pGameState->state_render_world_ui_tick(mainView, this->m_sfWindow, flDelta);

		//restore the default view, and begin GUI rendering
		this->m_sfWindow.setView(this->m_sfWindow.getDefaultView());

		//RENDER STATE UI HERE
		pGameState->state_render_ui_tick(mainView, this->m_sfWindow, flDelta);

		if(pGameState->state_render_ui())
		{
			ImGui::SFML::Update(this->m_sfWindow, imDeltaClock.restart());

			//do ImGui windows
			this->m_pInterfaceManager->render_all_panels(flDelta);

			//tick comm manager
			if(this->m_bGamePaused.load() == false)
			{
				this->m_pCommsManager->tick_comms(flDelta);
			}

			//render comms
			this->m_pCommsManager->render_comms();

			ImGui::SFML::Render(this->m_sfWindow);
		}

		//rendering operation completed. display the frame.
		this->m_sfWindow.display();

		//DO STATE POST RENDER TICK HERE
		pGameState->state_postrender_tick(this->m_sfWindow, flDelta);

		//UNLOCK the window access mutex
		this->m_mWindowAccess.unlock();

		if(this->m_bMainLoopContinue.load() == false)
		{
			bContinue = false;
		}

		lastTime = time;
	}
}

/*
 * handle physics, collisions, world events, etc..
 */
void CGame::enter_world_loop(void)
{
	bool bContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();

	while(bContinue)
	{
		auto time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> time_diff = time - lastTime;
		float flDelta = time_diff.count();

		if(this->m_bGamePaused.load() == false)
		{
			//update particles
			this->m_pParticleManager->update_particles(this->m_sfWindow, flDelta);

			//update world
			this->m_pWorld->world_tick(flDelta);
		}

		std::chrono::duration<float> sleep_duration = std::chrono::microseconds(250);

		std::this_thread::sleep_for(sleep_duration);

		if(this->m_bMainLoopContinue.load() == false)
		{
			bContinue = false;
		}

		lastTime = time;
	}
}

/*
 * this thread will drive the game's Lua engine.
 * 
 * script_tick() will tick pending callbacks' timers
 */
void CGame::enter_script_loop(void)
{
	try
	{
		this->m_pScriptEngine->initialize();
	}
	catch(SGException e)
	{
		this->core_fault(e);
	}

	bool bContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();

	while(bContinue)
	{
		auto time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> time_diff = time - lastTime;
		float flDelta = time_diff.count();

		if(this->m_bGamePaused.load() == false)
		{
			try
			{
				this->m_pScriptEngine->script_tick(flDelta);
			}
			catch(SGException e)
			{
				this->core_fault(e);
			}
		}

		std::chrono::duration<float> sleep_duration = std::chrono::milliseconds(1);

		std::this_thread::sleep_for(sleep_duration);

		if(this->m_bMainLoopContinue.load() == false)
		{
			bContinue = false;
		}

		lastTime = time;
	}
}

void CGame::core_fault(SGException const &e)
{
	std::cout << "Core fault: Unable to continue." << std::endl;
	std::cout << "Exception: " << e.get_exception() << std::endl;

	this->m_bMainLoopContinue.store(false);
}