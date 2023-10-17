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
#include "SectorMapPanel.hxx"
#include "SGLib.hxx"
#include "CObject.hxx"

int SectorMapPanel::m_iPanelInstances = 0;

SectorMapPanel::SectorMapPanel(SectorId const uiSector)
	: m_uiSectorId(uiSector), m_pSector(nullptr), m_pSectorMapRenderer(SG::get_render_pipeline()->get_sector_map_renderer())
{
	if(++SectorMapPanel::m_iPanelInstances > 1)
	{
		this->m_bPanelActive = false;
	}
	else
	{
		this->m_bPanelActive = true;

		if(uiSector)
		{
			m_pSector = &SG::get_universe()->get_sector(uiSector);
		}
	}
}

SectorMapPanel::~SectorMapPanel()
{
	--SectorMapPanel::m_iPanelInstances;
}

void SectorMapPanel::render_panel(float const flDelta)
{
	ImGui::Begin("Sector Map", nullptr, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

	if(this->m_pSector == nullptr)
	{
		ImGui::Text("Unknown location. No map data available.");
	}
	else
	{
		ImGui::Text(this->m_pSector->get_name().c_str());

		//try to get the player instance to update the position
		{
			InstanceId playerInstance = SG::get_world()->get_player_unchecked();

			if(playerInstance != INVALID_INSTANCE && SG::get_engine()->instance_is_allocated(playerInstance))
			{
				CObject* pObject = SG::get_engine()->instance_get_checked<CObject>(playerInstance);

				if(pObject != nullptr)
				{
					this->m_pSectorMapRenderer->update_player_position(pObject->get_position());

					this->m_pSectorMapRenderer->enable_player_icon(true);
				}
				else
				{
					this->m_pSectorMapRenderer->enable_player_icon(false);
				}
			}
			else
			{
				this->m_pSectorMapRenderer->enable_player_icon(false);
			}
		}

		this->m_pSectorMapRenderer->draw_map();

		void* pTextureHandle = this->m_pSectorMapRenderer->get_map_texture_handle();

		ImGui::Image(pTextureHandle, ImVec2(500.0f, 500.0f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	}

	if(ImGui::Button("Done"))
	{
		this->m_bPanelActive = false;
	}

	ImGui::End();
};