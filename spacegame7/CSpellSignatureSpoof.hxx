#pragma once

#include "ISpell.hxx"

class CSpellSignatureSpoof : public ISpell
{
public:
	virtual std::string get_name(void)
	{
		return "Return to Sender";
	};

	virtual std::string get_desc(void)
	{
		return "Hacks all missiles within 750m.\n 50% chance to detonate missile\n 50% chance to redesignate missile target";
	};

	virtual StaticTextureId get_icon_texture(void)
	{
		return 19;
	};

	virtual float get_cooldown(void)
	{
		return 10.0f;
	};

	virtual float get_mana_cost(void)
	{
		return 20.0f;
	};

	virtual bool cast(InstanceId const, InstanceId const);
private:
};