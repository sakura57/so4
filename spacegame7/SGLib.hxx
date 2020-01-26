#pragma once

#include <random>
#include "Defs.hxx"
#include "EngineProtos.hxx"
#include "IEngine.hxx"
#include "IWorld.hxx"
#include "IRenderPipeline.hxx"
#include "IScriptEngine.hxx"
#include "CGameDataManager.hxx"
#include "InterfaceManager.hxx"
#include "CAudioManager.hxx"

#include "CParticleManager.hxx"
#include "CCommsManager.hxx"
#include "CGameStateManager.hxx"
#include "CGameSettings.hxx"
#include "IStaticTextureManager.hxx"
#include "IUniverse.hxx"
#include "IIntransientDataManager.hxx"
#include "ILootTableManager.hxx"
#include "ISpellManager.hxx"
#include "IMatManager.hxx"
#include "IGameClock.hxx"

namespace SG
{
	IEngine *get_engine(void);
	IWorld *get_world(void);
	IRenderPipeline *get_render_pipeline(void);
	IScriptEngine *get_script_engine(void);
	CGameDataManager *get_game_data_manager(void);
	InterfaceManager *get_interface_manager(void);
	CAudioManager *get_audio_manager(void);
	CParticleManager *get_particle_manager(void);
	CCommsManager *get_comms_manager(void);
	CGameStateManager *get_game_state_manager(void);
	CGameSettings *get_game_settings(void);
	IStaticTextureManager *get_static_texture_manager(void);
	IUniverse *get_universe(void);
	IIntransientDataManager *get_intransient_data_manager(void);
	ILootTableManager *get_loot_table_manager(void);
	ISpellManager *get_spell_manager(void);
	IMatManager* get_material_manager(void);
	IGameClock* get_game_clock(void);

	std::mt19937 &get_random(void);
}