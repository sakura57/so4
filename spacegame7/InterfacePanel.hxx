#pragma once

#include "imgui.h"

class InterfacePanel
{
public:
	InterfacePanel()
	{};
	virtual ~InterfacePanel()
	{};

	//Render the interface panel.
	//This function is called during the render loop, after
	//world objects are rendered.
	virtual void render_panel(float const) = 0;

	//return whether the panel is active or not.
	//the interface manager will use this function
	//to determine if a panel should be destroyed
	virtual bool panel_active(void) = 0;

	/*
	 * Return true if this UI element should nullify
	 * input, false otherwise.
	 */
	virtual bool prevents_game_input(void) = 0;

private:
};