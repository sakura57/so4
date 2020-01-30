#pragma once

#include "InterfacePanel.hxx"

class InGameMenuPanel : public InterfacePanel
{
public:
	InGameMenuPanel();
	virtual ~InGameMenuPanel();

	virtual void render_panel(float const);

	virtual bool panel_active(void)
	{
		return this->m_bPanelActive;
	};

	virtual bool prevents_game_input(void)
	{
		return true;
	};
private:
	bool m_bPanelActive;
	static bool m_bPanelExists;
};