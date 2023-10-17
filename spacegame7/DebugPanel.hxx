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

#include <sstream>

#include "InterfacePanel.hxx"

#include "CEquippedObject.hxx"

class DebugPanel : public InterfacePanel
{
public:
	DebugPanel()
	{};
	virtual ~DebugPanel()
	{};

	virtual void render_panel(float const flDelta)
	{
		ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

		unsigned int uiInstances = SG::get_engine()->instance_get_count();
		std::stringstream ss;
		ss << "Instances: " << uiInstances << "/" << DEFAULT_POOL_SIZE;
		ImGui::Text(ss.str().c_str());
		ss.clear();
		ss.str(std::string());

		ss << "Particles: " << SG::get_particle_manager()->get_num_particles();
		ImGui::Text(ss.str().c_str());
		ss.clear();
		ss.str(std::string());

		ss << "Sounds: " << SG::get_audio_manager()->get_num_active_sounds();
		ImGui::Text(ss.str().c_str());

		ImGui::End();
	};

	virtual bool panel_active(void)
	{
		return true;
	};

	virtual bool prevents_game_input(void)
	{
		return false;
	};

private:
};