#include "SGLib.hxx"
#include "SGEngine.hxx"
#include "CGameWorld.hxx"
#include "CSFMLRenderPipeline.hxx"
#include "CSFMLStaticTextureManager.hxx"
#include "CLuaEngine.hxx"
#include "InterfaceManager.hxx"
#include "CUniverse.hxx"
#include "CIntransientDataManager.hxx"
#include "CLootTableManager.hxx"
#include "CSpellManager.hxx"
#include "CMatManager.hxx"
#include "CGameClock.hxx"

IEngine *SG::get_engine(void)
{
	static SGEngine engine;
	return &engine;
}

IWorld *SG::get_world(void)
{
	static CGameWorld world;
	return &world;
}

IRenderPipeline *SG::get_render_pipeline(void)
{
	static CSFMLRenderPipeline renderPipeline;
	return &renderPipeline;
}

IScriptEngine *SG::get_script_engine(void)
{
	static CLuaEngine luaEngine;
	return &luaEngine;
}

CGameDataManager *SG::get_game_data_manager(void)
{
	static CGameDataManager gameDataManager;
	return &gameDataManager;
}

InterfaceManager * SG::get_interface_manager(void)
{
	static InterfaceManager interfaceManager;
	return &interfaceManager;
}

CAudioManager * SG::get_audio_manager(void)
{
	static CAudioManager audioManager;
	return &audioManager;
}

CParticleManager * SG::get_particle_manager(void)
{
	static CParticleManager partManager;
	return &partManager;
}

std::mt19937 &SG::get_random(void)
{
	static std::mt19937 random(time(NULL));
	return random;
}

CCommsManager * SG::get_comms_manager(void)
{
	static CCommsManager commsManager;
	return &commsManager;
}

CGameStateManager *SG::get_game_state_manager(void)
{
	static CGameStateManager gameStateManager;
	return &gameStateManager;
}

CGameSettings *SG::get_game_settings(void)
{
	static CGameSettings gameSettings;
	return &gameSettings;
}

IStaticTextureManager *SG::get_static_texture_manager(void)
{
	static CSFMLStaticTextureManager staticTextureManager;
	return &staticTextureManager;
}

IUniverse *SG::get_universe(void)
{
	static CUniverse universe;
	return &universe;
}

IIntransientDataManager * SG::get_intransient_data_manager(void)
{
	static CIntransientDataManager intransientDataManager;
	return &intransientDataManager;
}

ILootTableManager *SG::get_loot_table_manager(void)
{
	static CLootTableManager lootTableManager;
	return &lootTableManager;
}

ISpellManager *SG::get_spell_manager(void)
{
	static CSpellManager spellManager;
	return &spellManager;
}

IMatManager* SG::get_material_manager(void)
{
	static CMatManager materialManager;
	return &materialManager;
}

IGameClock* SG::get_game_clock(void)
{
	static CGameClock gameClock;
	return &gameClock;
}