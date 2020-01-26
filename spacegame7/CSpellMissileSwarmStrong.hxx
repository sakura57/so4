#pragma once

#include "ISpell.hxx"

class CSpellMissileSwarmStrong : public ISpell
{
public:
	virtual std::string get_name(void)
	{
		return "Missile Swarm II";
	};

	virtual std::string get_desc(void)
	{
		return "Launches a swarm of\n strong missiles";
	};

	virtual StaticTextureId get_icon_texture(void)
	{
		return 21;
	};

	virtual float get_cooldown(void)
	{
		return 15.0f;
	};

	virtual float get_mana_cost(void)
	{
		return 150.0f;
	};

	virtual bool cast(InstanceId const, InstanceId const);
private:
};