#pragma once

#include "IStatusEffect.hxx"
#include "CTransientStatusEffect.hxx"

class CStatusNanobotStream : public CTransientStatusEffect
{
public:
	CStatusNanobotStream(float const flDuration)
	{
		this->m_flTimeRemaining = flDuration;
	};

	virtual StaticTextureId get_icon_texture(void) const
	{
		return 15;
	};

	virtual std::string get_name(void) const
	{
		return "Nanobot Stream";
	};

	virtual std::string get_desc(void) const
	{
		return "+1% HP/s";
	};

	virtual float get_hull_drain_percent(void) const
	{
		return -0.01f;
	};
};