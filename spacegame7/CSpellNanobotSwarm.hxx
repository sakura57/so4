#pragma once

#include "ISpell.hxx"

class CSpellNanobotSwarm : public ISpell
{
public:
	virtual std::string get_name(void)
	{
		return "Nanobot Swarm";
	};

	virtual std::string get_desc(void)
	{
		return "Restore 20% HP over 2s";
	};

	virtual StaticTextureId get_icon_texture(void)
	{
		return 16;
	};

	virtual float get_cooldown(void)
	{
		return 30.0f;
	};

	virtual float get_mana_cost(void)
	{
		return 50.0f;
	};

	virtual bool cast(InstanceId const, InstanceId const);
private:
};