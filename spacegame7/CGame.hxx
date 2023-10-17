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

#include <iostream>
#include <mutex>

#include "SFMLIncludes.hxx"

#include "imgui.h"
#include "imgui-sfml.h"

#include "Defs.hxx"
#include "IEngine.hxx"
#include "IRenderPipeline.hxx"
#include "IScriptEngine.hxx"
#include "IWorld.hxx"
#include "ICamera.hxx"
#include "CGameDataManager.hxx"
#include "CShip.hxx"
#include "InterfaceManager.hxx"
#include "CAudioManager.hxx"
#include "CGameStateManager.hxx"
#include "ILootTableManager.hxx"
#include "ICharacterEntityManager.hxx"
#include "IMatManager.hxx"
#include "IGameClock.hxx"

/*
 * The main game class.
 * 
 * The main loop is contained here,
 * among others.
 */
class CGame
{
public:
	CGame(void);
	~CGame(void);
	void enter_main_loop(void);
private:
	std::atomic_bool m_bMainLoopContinue;
	std::atomic_bool m_bGamePaused;

	unsigned int resX;
	unsigned int resY;

	IEngine * m_pEngine;
	IRenderPipeline * m_pRenderPipeline;
	IWorld * m_pWorld;
	IScriptEngine *m_pScriptEngine;
	CGameDataManager *m_pGameDataManager;
	InterfaceManager *m_pInterfaceManager;
	CAudioManager *m_pAudioManager;
	CParticleManager *m_pParticleManager;
	CCommsManager *m_pCommsManager;
	CGameStateManager *m_pGameStateManager;
	IStaticTextureManager *m_pStaticTextureManager;
	IUniverse *m_pUniverse;
	ILootTableManager *m_pLootTableManager;
	ICharacterEntityManager *m_pCharEntityManager;
	IMatManager* m_pMaterialManager;
	IGameClock* m_pGameClock;
	IDebugConsole* m_pDebugConsole;
	CDialogueManager* m_pDialogueManager;

	sf::RenderWindow m_sfWindow;
	std::mutex m_mWindowAccess;

	void enter_world_loop(void);
	void enter_render_loop(void);
	void enter_script_loop(void);
	void load_data_delegate(void);

	void core_fault(SGException const &e);
};