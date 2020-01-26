#pragma once

#include "ISpell.hxx"

class CSpellLaserBarrageStrong : public ISpell
{
public:
	virtual std::string get_name(void)
	{
		return "Laser Cascade II";
	};

	virtual std::string get_desc(void)
	{
		return "Fires several waves of\n strong lasers";
	};

	virtual StaticTextureId get_icon_texture(void)
	{
		return 25;
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