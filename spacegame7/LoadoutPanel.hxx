#pragma once

#include <sstream>

#include "CEquippedObject.hxx"
#include "InterfacePanel.hxx"

class LoadoutPanel : public InterfacePanel
{
public:
	LoadoutPanel(InstanceId const iObjectId)
		: m_iObjectId(iObjectId)
	{};
	virtual ~LoadoutPanel()
	{};

	virtual void render_panel(float const);

	static void do_item_tooltip(EquipArch const*);

	virtual bool panel_active(void)
	{
		return true;
	};

	virtual bool prevents_game_input(void)
	{
		return false;
	};

private:
	InstanceId const m_iObjectId;
};