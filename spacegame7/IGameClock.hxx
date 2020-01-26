#pragma once

#include "Defs.hxx"

/*
 * Abstraction of game clock class.
 */
interface IGameClock
{
	virtual void tick(float const) = 0;
	virtual float get_absolute_delta(void) = 0;
};