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