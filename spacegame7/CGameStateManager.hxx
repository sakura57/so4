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

#include "Util.hxx"
#include "IGameState.hxx"

class CGameStateManager
{
public:
	CGameStateManager();
	~CGameStateManager();

	void transition_game_state(IGameState *);
	IGameState *get_game_state(void);

	bool has_pending_game_state(void);
	void shift_to_pending_state(void);

	void notify_initiate_loading(void);
	void notify_loading_finished(void);

private:
	Spinlock m_mFieldAccess;
	IGameState *m_currentState;
	IGameState *m_pendingState;
	IGameState* m_loadingState;
	bool m_bLoading;

	void flush_semitransient_states(void);
};