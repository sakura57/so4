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

#include "InterfacePanel.hxx"
#include "IRadarVisible.hxx"
#include "CGameWorld.hxx"
#include <sstream>
#include "CWorldTransaction.hxx"

class RadarPanel : public InterfacePanel
{
public:
	RadarPanel(InstanceId const iObjectId)
		: m_iObjectId(iObjectId)
	{};
	virtual ~RadarPanel()
	{};

	virtual void render_panel(float const flDelta)
	{
		CWorldTransaction wTransaction;

		if(!SG::get_engine()->instance_is_allocated(this->m_iObjectId))
		{
			return;
		}

		CShip *pWorldObject = SG::get_engine()->instance_get_checked<CShip>(this->m_iObjectId);

		if(pWorldObject == nullptr)
		{
			return;
		}

		ImGui::Begin("Radar", nullptr, ImGuiWindowFlags_NoFocusOnAppearing);

		IWorld *pGameWorld = SG::get_world();

		int instances = 0;

		/*
		 * we will walk instances. if they implement RadarVisible,
		 * show them in the object list
		 */
		IWorldInstance *pInstanceWalk = pGameWorld->instance_walk_begin();

		if(pInstanceWalk == nullptr)
		{
			goto NO_INSTANCES_TO_RENDER;
		}

		do
		{
			if(pInstanceWalk == pWorldObject) continue;

			InstanceFlags uiFlags = pInstanceWalk->instance_get_flags();

			//does this instance implement IRadarVisible?
			if(uiFlags & IWorldObject::InstanceFlag)
			{
				IWorldObject * pWalkWorldObject = static_cast<IWorldObject*>(pInstanceWalk);

				IRadarVisible *pRadarObject = pWalkWorldObject->get_radar();

				if(pRadarObject != nullptr)
				{
					IRadarVisible::RadarShape radarShape = pRadarObject->get_radar_shape();

					float fDistance = pWorldObject->get_position().distance(pWalkWorldObject->get_position());

					if(fDistance < 2500.0f)
					{
						bool bSelected = pWorldObject->get_target() == pWalkWorldObject->instance_get_id();

						/*
						std::stringstream ss;
						ss << pRadarObject->get_radar_name() << " (" << std::setprecision(0) << fDistance << ")";

						std::string szRadarName(ss.str());
						*/

						ImGui::PushID(pWalkWorldObject->instance_get_id());
						if(ImGui::Selectable(pRadarObject->get_radar_name().c_str(), &bSelected))
						{
							pWorldObject->set_target(pWalkWorldObject->instance_get_id());

							SG::get_audio_manager()->play_sound(30U);
						}

						ImGui::PopID();
					}
				}
			}

		} while (pInstanceWalk = pGameWorld->instance_walk_next());
	NO_INSTANCES_TO_RENDER:

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
	InstanceId const m_iObjectId;
};
