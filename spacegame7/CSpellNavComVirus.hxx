#pragma once

#include "ISpell.hxx"

class CSpellNavComVirus : public ISpell
{
public:
	virtual std::string get_name(void)
	{
		return "NavCom Virus";
	};

	virtual std::string get_desc(void)
	{
		return "Disable target\'s engine for 5s";
	};

	virtual StaticTextureId get_icon_texture(void)
	{
		return 13;
	};

	virtual float get_cooldown(void)
	{
		return 10.0f;
	};

	virtual float get_mana_cost(void)
	{
		return 100.0f;
	};

	virtual bool cast(InstanceId const, InstanceId const);
private:
};