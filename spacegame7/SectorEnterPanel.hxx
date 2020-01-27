#pragma once

#include <sstream>

#include "CInSpaceState.hxx"
#include "IUniverse.hxx"
#include "SGLib.hxx"
#include "InterfacePanel.hxx"

#define BUFFER_SIZE 512
#define LINE_LENGTH 24
#define LINES_PER_BOX 12

class SectorEnterPanel : public InterfacePanel
{
public:
	SectorEnterPanel(SectorId const iSectorId, std::string const &szRmsnScript, Vector2f vStartingPosition)
		: m_iSectorId(iSectorId), m_pUniverse(SG::get_universe()), m_sector(m_pUniverse->get_sector(m_iSectorId)), m_vStartingPosition(vStartingPosition), m_szRmsnScript(szRmsnScript)
	{
		CGameDataManager *pGameDataManager = SG::get_game_data_manager();

		this->m_pSectorGoverningFaction = pGameDataManager->get_faction(this->m_sector.get_governing_faction());

		/*
		 * Short snippet to add line breaks to description
		 */
		std::string str("");
		std::string szDesc(m_sector.get_desc().c_str());

		bool bCancelLine = false;

		for(int i = 0; i < m_sector.get_desc().size(); ++i)
		{
			if(i && i % LINE_LENGTH == 0)
			{
				bCancelLine = true;
			}

			if(bCancelLine && szDesc[i] == ' ')
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

	virtual ~SectorEnterPanel()
	{};

	virtual void render_panel(float const flDelta)
	{
		ImGui::SetNextWindowPosCenter(ImGuiCond_Once | ImGuiCond_Appearing);

		ImVec2 displaySize = ImGui::GetIO().DisplaySize;

		ImVec2 windowSize(displaySize.x / 3.0f, displaySize.y / 2.0f);

		ImGui::Begin(
			"Entering Sector", nullptr, windowSize, -1.0F,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove
		);

		ImGui::Text("Name: ");
		ImGui::SameLine();
		ImGui::Text(this->m_sector.get_name().c_str());

		ImGui::Text("Governing Faction: ");
		ImGui::SameLine();
		ImGui::Text(this->m_pSectorGoverningFaction->name.c_str());

		ImGui::Text("Description:");
		ImVec2 BufSize = ImGui::CalcTextSize(this->m_szDescBuffer);
		ImGui::InputTextMultiline("##desc", this->m_szDescBuffer, BUFFER_SIZE, ImVec2(BufSize.x + 16.0f, ImGui::GetTextLineHeight() * LINES_PER_BOX), ImGuiInputTextFlags_ReadOnly);

		ImGui::Separator();

		ImGui::Text("You are now entering this sector.");

		if(ImGui::Button("Confirm"))
		{
			std::string szSectorScript = this->m_sector.get_script_path();

			CInSpaceState* pNewState = new CInSpaceState(szSectorScript.c_str(), this->m_iSectorId, this->m_vStartingPosition);

			if(!this->m_szRmsnScript.empty())
			{
				pNewState->set_rmsn_script(m_szRmsnScript);
			}

			SG::get_game_state_manager()->transition_game_state(pNewState);
		}

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
	SectorId m_iSectorId;
	IUniverse *m_pUniverse;
	CSector const &m_sector;
	Faction const *m_pSectorGoverningFaction;
	char m_szDescBuffer[BUFFER_SIZE];
	Vector2f m_vStartingPosition;
	std::string m_szRmsnScript;
};