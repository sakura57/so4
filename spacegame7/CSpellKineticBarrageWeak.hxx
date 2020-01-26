#pragma once

#include "ISpell.hxx"

class CSpellKineticBarrageWeak : public ISpell
{
public:
	virtual std::string get_name(void)
	{
		return "Kinetic Barrage I";
	};

	virtual std::string get_desc(void)
	{
		return "Fires a barrage of light\n kinetic projectiles";
	};

	virtual StaticTextureId get_icon_texture(void)
	{
		return 22;
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