/**
 * Copyright (C) 2020 Jacob Farnsworth.
 *
 * This file is part of the Spaced Out 4 project.
 *
 * Spaced Out 4 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, version 2.
 *
 * Spaced Out 4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */
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
#include "IDebugConsole.hxx"
#include "CDialogueManager.hxx"
#include "IMarketManager.hxx"

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
	IDebugConsole* get_debug_console(void);
	CDialogueManager* get_dialogue_manager(void);
	IMarketManager* get_market_manager(void);

	std::mt19937 &get_random(void);
}