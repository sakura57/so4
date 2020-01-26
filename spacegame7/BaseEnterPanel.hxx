#pragma once

#include <sstream>

#include "CLandedState.hxx"
#include "IUniverse.hxx"
#include "SGLib.hxx"
#include "InterfacePanel.hxx"

#define BUFFER_SIZE 512
#define LINE_LENGTH 24
#define LINES_PER_BOX 12

class BaseEnterPanel : public InterfacePanel
{
public:
	BaseEnterPanel(BaseId const iBaseId)
		: m_iBaseId(iBaseId), m_pUniverse(SG::get_universe()), m_base(m_pUniverse->get_base(m_iBaseId))
	{
		CGameDataManager *pGameDataManager = SG::get_game_data_manager();

		this->m_pBaseFaction = pGameDataManager->get_faction(this->m_base.get_faction());

		/*
		* Short snippet to add line breaks to description
		*/
		std::string str("");
		std::string szDesc(m_base.get_description().c_str());

		bool bCancelLine = false;

		for(int i = 0; i < szDesc.size(); ++i)
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

	~BaseEnterPanel()
	{};

	virtual void render_panel(float const flDelta)
	{
		ImGui::SetNextWindowPosCenter(ImGuiCond_Once | ImGuiCond_Appearing);

		ImVec2 displaySize = ImGui::GetIO().DisplaySize;

		ImVec2 windowSize(displaySize.x / 3.0f, displaySize.y / 2.0f);

		ImGui::Begin(
			"Entering Base", nullptr, windowSize, -1.0F,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove
		);

		ImGui::Text("Name: ");
		ImGui::SameLine();
		ImGui::Text(this->m_base.get_name().c_str());

		ImGui::Text("Operated By: ");
		ImGui::SameLine();
		ImGui::Text(this->m_pBaseFaction->name.c_str());

		ImGui::Text("Description:");
		ImVec2 BufSize = ImGui::CalcTextSize(this->m_szDescBuffer);
		ImGui::InputTextMultiline("##desc", this->m_szDescBuffer, BUFFER_SIZE, ImVec2(BufSize.x + 16.0f, ImGui::GetTextLineHeight() * LINES_PER_BOX), ImGuiInputTextFlags_ReadOnly);

		ImGui::Separator();

		ImGui::Text("You are now entering this base.");

		if(ImGui::Button("Confirm"))
		{
			//std::string szSectorScript = this->m_base.get_script_path();

			SG::get_game_state_manager()->transition_game_state(new CLandedState(this->m_iBaseId));
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
	BaseId m_iBaseId;
	IUniverse *m_pUniverse;
	CBase const &m_base;
	Faction const *m_pBaseFaction;
	char m_szDescBuffer[BUFFER_SIZE];
};