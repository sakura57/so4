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