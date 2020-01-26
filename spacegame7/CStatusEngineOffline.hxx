#pragma once

#include "IStatusEffect.hxx"
#include "CTransientStatusEffect.hxx"

class CStatusEngineOffline : public CTransientStatusEffect
{
public:
	CStatusEngineOffline(float const flDuration)
	{
		this->m_flTimeRemaining = flDuration;
	};

	virtual StaticTextureId get_icon_texture(void) const
	{
		return 12;
	};

	virtual std::string get_name(void) const
	{
		return "Engine Offline";
	};

	virtual std::string get_desc(void) const
	{
		return "-100% engine power";
	};

	virtual float get_engine_thrust_multiplier(void) const
	{
		return 0.0f;
	};
};