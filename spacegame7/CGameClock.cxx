#include "CGameClock.hxx"

CGameClock::CGameClock()
	: m_flAbsoluteDelta(0.0f)
{

}

CGameClock::~CGameClock()
{

}

void CGameClock::tick(float const flDelta)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flAbsoluteDelta += flDelta;
}

float CGameClock::get_absolute_delta(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_flAbsoluteDelta;
}