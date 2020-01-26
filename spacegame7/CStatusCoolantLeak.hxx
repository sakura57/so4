#pragma once

#include "IStatusEffect.hxx"
#include "CTransientStatusEffect.hxx"

class CStatusCoolantLeak : public CTransientStatusEffect
{
public:
	CStatusCoolantLeak(float const flDuration)
	{
		this->m_flTimeRemaining = flDuration;
	};

	virtual StaticTextureId get_icon_texture(void) const
	{
		return 11;
	};

	virtual std::string get_name(void) const
	{
		return "Coolant Leak";
	};

	virtual std::string get_desc(void) const
	{
		return "-0.5% HP per second\n-5% engine power";
	};

	virtual float get_hull_drain_percent(void) const
	{
		return 0.005f;
	};

	virtual float get_engine_thrust_multiplier(void) const
	{
		return 0.95f;
	};
};