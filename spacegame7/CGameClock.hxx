#pragma once

#include "IGameClock.hxx"
#include "Util.hxx"

/*
 * Implementation of game clock class.
 * This is instantiated as a static global in SGLib
 * while tick() is called each frame by the render thread.
 *
 * get_absolute_delta() can be called at any time, from any thread
 * returning the time passed since this class was instantiated,
 * which is usually around the time the game was started.
 */
class CGameClock : public IGameClock
{
public:
	CGameClock();
	~CGameClock();

	virtual void tick(float const);
	virtual float get_absolute_delta(void);

private:
	Spinlock m_mFieldAccess;
	float m_flAbsoluteDelta;
};