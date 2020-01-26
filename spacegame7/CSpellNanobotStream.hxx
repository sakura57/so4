#pragma once

#include "ISpell.hxx"

class CSpellNanobotStream : public ISpell
{
public:
	virtual std::string get_name(void)
	{
		return "Nanobot Stream";
	};

	virtual std::string get_desc(void)
	{
		return "Restore 10% HP over 10s";
	};

	virtual StaticTextureId get_icon_texture(void)
	{
		return 15;
	};

	virtual float get_cooldown(void)
	{
		return 20.0f;
	};

	virtual float get_mana_cost(void)
	{
		return 30.0f;
	};

	virtual bool cast(InstanceId const, InstanceId const);
private:
};