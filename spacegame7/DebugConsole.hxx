#pragma once

#include <sstream>

#include "InterfacePanel.hxx"

class DebugConsole : public InterfacePanel
{
public:
	DebugConsole()
	{};
	virtual ~DebugConsole()
	{};

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void)
	{
		return true;
	};

	virtual bool prevents_game_input(void)
	{
		return true;
	};

private:
};