#pragma once

#include "ISpell.hxx"

class CSpellPerfectDefense : public ISpell
{
public:
	virtual std::string get_name(void)
	{
		return "Perfect Defense";
	};

	virtual std::string get_desc(void)
	{
		return "Completely immune to weapons\n damage for 5s";
	};

	virtual StaticTextureId get_icon_texture(void)
	{
		return 20;
	};

	virtual float get_cooldown(void)
	{
		return 120.0f;
	};

	virtual float get_mana_cost(void)
	{
		return 400.0f;
	};

	virtual bool cast(InstanceId const, InstanceId const);
private:
};