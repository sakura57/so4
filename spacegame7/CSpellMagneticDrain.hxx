#pragma once

#include "ISpell.hxx"

class CSpellMagneticDrain : public ISpell
{
public:
	virtual std::string get_name(void)
	{
		return "Magnetic Drain";
	};

	virtual std::string get_desc(void)
	{
		return "Steal up to 40% capacitor\n energy from the target";
	};

	virtual StaticTextureId get_icon_texture(void)
	{
		return 18;
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