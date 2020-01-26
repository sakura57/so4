#pragma once

#include "IStatusEffect.hxx"
#include "CTransientStatusEffect.hxx"

class CStatusHullBreach : public CTransientStatusEffect
{
public:
	CStatusHullBreach(float const flDuration)
	{
		this->m_flTimeRemaining = flDuration;
	};

	virtual StaticTextureId get_icon_texture(void) const
	{
		return 10;
	};

	virtual std::string get_name(void) const
	{
		return "Hull Breach";
	};
	
	virtual std::string get_desc(void) const
	{
		return "+5% hull damage received";
	};

	virtual float get_hull_damage_multiplier(void) const
	{
		return 1.05f;
	};
};