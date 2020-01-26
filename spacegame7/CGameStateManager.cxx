#include <mutex>
#include "CGameStateManager.hxx"
#include "SGLib.hxx"

CGameStateManager::CGameStateManager()
{
	this->m_currentState = nullptr;
}

CGameStateManager::~CGameStateManager()
{
}

void CGameStateManager::transition_game_state(IGameState *pNewState)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if (this->m_pendingState != nullptr)
	{
		throw SGException("attempt to transition game state with another state already pending");
	}

	this->m_pendingState = pNewState;
}

IGameState *CGameStateManager::get_game_state(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_currentState;
}

bool CGameStateManager::has_pending_game_state(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_pendingState != nullptr;
}

void CGameStateManager::shift_to_pending_state(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if (this->m_pendingState == nullptr)
	{
		throw SGException("attempt to shift game state to a null pending state");
	}

	if (this->m_currentState != nullptr)
	{
		this->m_currentState->state_terminating();

		delete this->m_currentState;
	}

	this->flush_semitransient_states();

	this->m_currentState = this->m_pendingState;
	this->m_pendingState = nullptr;

	this->m_currentState->state_initializing();
}

void CGameStateManager::flush_semitransient_states(void)
{
	ITransientStateStructure *transientStates[] =
	{
		SG::get_audio_manager(),
		SG::get_comms_manager(),
		SG::get_interface_manager(),
		SG::get_particle_manager(),
		SG::get_script_engine(),
		SG::get_world()
	};

	for (ITransientStateStructure *transientState : transientStates)
	{
		transientState->shifting_out();
	}
}
