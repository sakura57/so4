/**
 * Copyright (C) 2020 Jacob Farnsworth.
 *
 * This file is part of the Spaced Out 4 project.
 *
 * Spaced Out 4 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, version 2.
 *
 * Spaced Out 4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */
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