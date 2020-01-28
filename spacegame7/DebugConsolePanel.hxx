#pragma once

#include <sstream>

#include "InterfacePanel.hxx"

class DebugConsolePanel : public InterfacePanel
{
public:
	DebugConsolePanel();
	virtual ~DebugConsolePanel();

	virtual void render_panel(float const flDelta);

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
	bool m_bScrollToBottom;
	static bool m_bPanelExists;
	char *m_szInputBuffer;
};