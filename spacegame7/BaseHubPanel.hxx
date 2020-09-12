#pragma once

#include "InterfacePanel.hxx"
#include "SGLib.hxx"
#include "CInSpaceState.hxx"
#include "CSectorTransitionState.hxx"
#include "SettingsPanel.hxx"
#include "EquipTraderPanel.hxx"
#include "JunkTraderPanel.hxx"
#include "MaterialTraderPanel.hxx"

#define BUFFER_SIZE 2048
#define LINE_LENGTH 24
#define LINES_PER_BOX 12

class BaseHubPanel : public InterfacePanel
{
public:
	BaseHubPanel(BaseId iBase)
		: m_bPanelActive(true), m_iBaseId(iBase), m_base(SG::get_universe()->get_base(iBase)), m_bDoRmsnOnLaunch(false), m_bShowMissionBoard(false)
	{
		/*
		 * Short snippet to add line breaks to description
		 */
		std::string str("");
		std::string szDesc(m_base.get_rmsn_desc().c_str());

		bool bCancelLine = false;

		for (int i = 0; i < m_base.get_rmsn_desc().size(); ++i)
		{
			if (i && i % LINE_LENGTH == 0)
			{
				bCancelLine = true;
			}

			if (bCancelLine && szDesc[i] == ' ')
			{
				str.push_back('\n');
				bCancelLine = false;
			}
			else
			{
				str.push_back(szDesc[i]);
			}
		}

		strcpy_s(this->m_szDescBuffer, BUFFER_SIZE, str.c_str());
	};
	virtual ~BaseHubPanel()
	{};

	virtual void render_panel(float const flDelta)
	{
		ImGui::SetNextWindowPosCenter(ImGuiCond_Once | ImGuiCond_Appearing);

		ImVec2 displaySize = ImGui::GetIO().DisplaySize;

		ImVec2 windowSize(displaySize.x / 8.0f, displaySize.y / 4.0f);

		ImGui::Begin(
			"Base Hub", nullptr, windowSize, -1.0F,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove
		);

		float windowWidth = ImGui::GetWindowWidth();

		ImVec2 buttonSize(windowWidth - windowWidth / 4.0f, 0.0f);
		ImGui::SetCursorPosX(windowWidth / 8.0f);

		if(ImGui::Button("Equipment Trader", buttonSize))
		{
			//SG::get_interface_manager()->free_all_panels();

			SG::get_interface_manager()->add_panel(new EquipTraderPanel(this->m_iBaseId));
		}

		ImGui::SetCursorPosX(windowWidth / 8.0f);
		if(ImGui::Button("Junk Trader", buttonSize))
		{
			//SG::get_interface_manager()->free_all_panels();

			SG::get_interface_manager()->add_panel(new JunkTraderPanel(this->m_iBaseId));
		}

		ImGui::SetCursorPosX(windowWidth / 8.0f);
		if(ImGui::Button("Material Trader", buttonSize))
		{
			//SG::get_interface_manager()->free_all_panels();

			SG::get_interface_manager()->add_panel(new MaterialTraderPanel(this->m_iBaseId));
		}

		if (m_base.get_rmsn_offered())
		{
			ImGui::SetCursorPosX(windowWidth / 8.0f);
			if (ImGui::Button("Mission Board", buttonSize))
			{
				//SG::get_interface_manager()->free_all_panels();

				m_bShowMissionBoard = true;
			}
		}

		ImGui::Separator();

		ImGui::SetCursorPosX(windowWidth / 8.0f);
		if(ImGui::Button("Launch", buttonSize))
		{
			std::string szRmsnString("");

			if (this->m_bDoRmsnOnLaunch)
			{
				szRmsnString = this->m_base.get_rmsn_script();
			}

			//Save the game every time we enter a base
			std::string saveFileName = ".\\saves\\";
			saveFileName.append(SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity()->get_name());
			saveFileName.append(".sav");
			SG::get_game_data_manager()->dump_to_save(saveFileName, this->m_iBaseId);

			SG::get_game_state_manager()->transition_game_state(new CSectorTransitionState(this->m_base.get_launch_sector(), szRmsnString, this->m_base.get_launch_pos()));
		}

		ImGui::End();

		if (this->m_bShowMissionBoard)
		{
			ImGui::Begin("Mission Board");
			ImGui::Text("Mission Offered: %s", this->m_base.get_rmsn_name().c_str());
			ImGui::Separator();
			ImGui::Text("Reward: %d", this->m_base.get_rmsn_reward());
			ImGui::Text("Description:");
			ImVec2 BufSize = ImGui::CalcTextSize(this->m_szDescBuffer);
			ImGui::InputTextMultiline("##rmsndesc", this->m_szDescBuffer, BUFFER_SIZE, ImVec2(BufSize.x + 16.0f, ImGui::GetTextLineHeight() * LINES_PER_BOX), ImGuiInputTextFlags_ReadOnly);
			ImGui::Separator();
			if (ImGui::Button("Accept"))
			{
				this->m_bDoRmsnOnLaunch = true;
				this->m_bShowMissionBoard = false;
			}
			if (ImGui::Button("Decline"))
			{
				this->m_bShowMissionBoard = false;
			}
			ImGui::End();
		}
	};

	virtual bool panel_active(void)
	{
		return m_bPanelActive;
	};

	virtual bool prevents_game_input(void)
	{
		return false;
	};

private:
	bool m_bPanelActive;

	BaseId m_iBaseId;
	CBase const &m_base;
	char m_szDescBuffer[BUFFER_SIZE];

	bool m_bDoRmsnOnLaunch;
	bool m_bShowMissionBoard;
};