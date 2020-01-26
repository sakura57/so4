#pragma once

#include <sstream>

#include "InterfacePanel.hxx"
#include "ICharacterEntity.hxx"

struct AbilityDrop
{
	SpellId iSpellId;
	int iSpellSlot;
};

class AbilityToolbar : public InterfacePanel
{
public:
	AbilityToolbar(InstanceId iInstanceId, ICharacterEntity *pCharEntity)
		: m_pCharEntity(pCharEntity), m_iInstanceId(iInstanceId)
	{
	};
	virtual ~AbilityToolbar()
	{};

	virtual void render_panel(float const flDelta);

	static void do_ability_tooltip(ISpell *pSpell);

	virtual bool panel_active(void)
	{
		return true;
	};

	virtual bool prevents_game_input(void)
	{
		return false;
	};

private:
	ICharacterEntity *m_pCharEntity;
	InstanceId m_iInstanceId;
};