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
#include "IStatusEffect.hxx"

/*
 * When implementing a CTransientStatusEffect,
 * remember to initialize m_flTimeRemaining in the constructor
 */
class CTransientStatusEffect : public IStatusEffect
{
public:
	virtual void tick(float const flDelta)
	{
		SCOPE_LOCK(m_mFieldAccess);

		this->m_flTimeRemaining -= flDelta;
	};

	virtual bool expired(void)
	{
		SCOPE_LOCK(m_mFieldAccess);

		return this->m_flTimeRemaining < 0.0f;
	};

	virtual float get_hull_damage_multiplier(void) const
	{
		return 1.0f;
	};

	virtual float get_shield_damage_multiplier(void) const
	{
		return 1.0f;
	};

	virtual float get_hull_drain_percent(void) const
	{
		return 0.0f;
	};

	virtual float get_shield_drain_percent(void) const
	{
		return 0.0f;
	};

	virtual float get_capacitor_drain_percent(void) const
	{
		return 0.0f;
	};

	virtual float get_engine_thrust_multiplier(void) const
	{
		return 1.0f;
	};

protected:
	float m_flTimeRemaining;

private:
	Spinlock m_mFieldAccess;
};