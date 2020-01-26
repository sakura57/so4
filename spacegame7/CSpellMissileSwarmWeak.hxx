#pragma once

#include "ISpell.hxx"

class CSpellMissileSwarmWeak : public ISpell
{
public:
	virtual std::string get_name(void)
	{
		return "Missile Swarm I";
	};

	virtual std::string get_desc(void)
	{
		return "Launches a swarm of\n weak missiles";
	};

	virtual StaticTextureId get_icon_texture(void)
	{
		return 26;
	};

	virtual float get_cooldown(void)
	{
		return 10.0f;
	};

	virtual float get_mana_cost(void)
	{
		return 50.0f;
	};

	virtual bool cast(InstanceId const, InstanceId const);
private:
};