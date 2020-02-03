#pragma once

#include <future>

#include "SGLib.hxx"
#include "ICharacterEntity.hxx"
#include "LoadGamePanel.hxx"
#include "CSectorTransitionState.hxx"

bool LoadGamePanel::m_bPanelExists = false;

LoadGamePanel::LoadGamePanel()
	: m_bLoadingOperationInitiated(false)
{
	if(this->m_bPanelExists)
	{
		this->m_bPanelActive = false;
	}
	else
	{
		this->m_bPanelActive = true;
		this->m_bPanelExists = true;
	}
}

LoadGamePanel::~LoadGamePanel()
{
	this->m_bPanelExists = false;
}

void LoadGamePanel::render_panel(float const flDelta)
{
	if (!this->m_bPanelExists)
	{
		return;
	}

	static std::string loadFailedPopupText("");

	ImGui::SetNextWindowPosCenter(ImGuiCond_Once | ImGuiCond_Appearing);

	ImVec2 displaySize = ImGui::GetIO().DisplaySize;

	ImVec2 windowSize(displaySize.x / 8.0f, displaySize.y / 4.0f);

	ImGui::Begin(
		"Load Game", nullptr, windowSize, -1.0F,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	static char szCharName[16] = "Recruit";

	ImGui::InputText("Name", szCharName, 16, ImGuiInputTextFlags_CharsNoBlank);

	if(!this->m_bLoadingOperationInitiated)
	{
		if(ImGui::Button("Load"))
		{
			if(CGameDataManager::get_directory_exists("saves"))
			{
				std::stringstream ss;
				ss << "saves\\";
				ss << szCharName;
				ss << ".sav";

				this->m_fLoadingOperationResult = SG::get_game_data_manager()->load_from_save(ss.str());
				this->m_bLoadingOperationInitiated = true;
			}
			else
			{
				ImGui::OpenPopup("Load Failed");

				loadFailedPopupText = "Saves directory does not exist.\nPlease create the directory and try again.";
			}
		}
	}
	else
	{
		//check async load operation status
		if(this->m_fLoadingOperationResult.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			if(this->m_fLoadingOperationResult.get() == true)
			{
				//the load operation succeeded.
				//in practice, this piece of code will probably never be reached,
				//because the interface manager will be shifted out as a result of the
				//load operation, but just in case, we will delete ourself.

				this->m_bPanelActive = false;
			}
			else
			{
				ImGui::OpenPopup("Load Failed");

				loadFailedPopupText = "Save file not found, or is invalid.";
			}

			this->m_bLoadingOperationInitiated = false;
		}
	}

	if(ImGui::BeginPopupModal("Load Failed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Load game failed.\n");
		ImGui::Text(loadFailedPopupText.c_str());
		ImGui::Separator();

		if(ImGui::Button("Okay", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::End();
}

bool LoadGamePanel::panel_active(void)
{
	return this->m_bPanelActive;
}