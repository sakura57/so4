#pragma once

#include "ISpell.hxx"

class CSpellLaserBarrageWeak : public ISpell
{
public:
	virtual std::string get_name(void)
	{
		return "Laser Cascade I";
	};

	virtual std::string get_desc(void)
	{
		return "Fires several waves of\n weak lasers";
	};

	virtual StaticTextureId get_icon_texture(void)
	{
		return 24;
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