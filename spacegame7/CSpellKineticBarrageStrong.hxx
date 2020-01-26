#pragma once

#include "ISpell.hxx"

class CSpellKineticBarrageStrong : public ISpell
{
public:
	virtual std::string get_name(void)
	{
		return "Kinetic Barrage II";
	};

	virtual std::string get_desc(void)
	{
		return "Fires a barrage of heavy\n kinetic projectiles";
	};

	virtual StaticTextureId get_icon_texture(void)
	{
		return 23;
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