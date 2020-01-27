#include "SectorMapPanel.hxx"
#include "SGLib.hxx"
#include "CObject.hxx"

bool SectorMapPanel::m_bPanelExists = false;

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
		this->m_bPanelExists = false;
	}

	ImGui::End();
};