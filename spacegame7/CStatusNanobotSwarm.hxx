#pragma once

#include "IStatusEffect.hxx"
#include "CTransientStatusEffect.hxx"

class CStatusNanobotSwarm : public CTransientStatusEffect
{
public:
	CStatusNanobotSwarm(float const flDuration)
	{
		this->m_flTimeRemaining = flDuration;
	};

	virtual StaticTextureId get_icon_texture(void) const
	{
		return 16;
	};

	virtual std::string get_name(void) const
	{
		return "Nanobot Swarm";
	};

	virtual std::string get_desc(void) const
	{
		return "+10% HP/s";
	};

	virtual float get_hull_drain_percent(void) const
	{
		return -0.10f;
	};
};