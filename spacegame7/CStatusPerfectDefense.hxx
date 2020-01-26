#pragma once

#include "IStatusEffect.hxx"
#include "CTransientStatusEffect.hxx"

class CStatusPerfectDefense : public CTransientStatusEffect
{
public:
	CStatusPerfectDefense(float const flDuration)
	{
		this->m_flTimeRemaining = flDuration;
	};

	virtual StaticTextureId get_icon_texture(void) const
	{
		return 20;
	};

	virtual std::string get_name(void) const
	{
		return "Perfect Defense";
	};

	virtual std::string get_desc(void) const
	{
		return "-100% weapons dmg taken";
	};

	virtual float get_hull_damage_multiplier(void) const
	{
		return 0.0f;
	};

	virtual float get_shield_damage_multiplier(void) const
	{
		return 0.0f;
	};
};