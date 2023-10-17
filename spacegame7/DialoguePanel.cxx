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
#include "DialoguePanel.hxx"
#include "SGLib.hxx"


#define DIALOGUE_TIME_BETWEEN_CHARACTERS 0.035f

int DialoguePanel::m_iPanelInstances = 0;

DialoguePanel::DialoguePanel(DialogueLineId const uiDialogueLine)
	: m_bScrollToBottom(false), m_flDialogueTimer(0.0f), m_bDialogueInitialized(false), m_bDialogueFinishedPrinting(false)
{
	this->m_uiDialogueLine = uiDialogueLine;

	if(++this->m_iPanelInstances > 1)
	{
		this->m_bPanelActive = false;
	}
	else
	{
		this->m_bPanelActive = true;
	}
}

DialoguePanel::~DialoguePanel()
{
	--this->m_iPanelInstances;
}

void DialoguePanel::render_panel(float const flDelta)
{
	ImGui::SetNextWindowPosCenter();
	ImGui::SetNextWindowSize(ImVec2(420, 400));

	ImGui::Begin("Dialogue", nullptr, ImVec2(420, 400), -1.0f, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

	if(!this->m_bDialogueInitialized)
	{
		return;
	}

	if(this->m_uiCharactersShown < this->m_szDialogueText.size())
	{
		this->m_flDialogueTimer += flDelta;

		if(this->m_flDialogueTimer > DIALOGUE_TIME_BETWEEN_CHARACTERS)
		{
			this->m_flDialogueTimer = 0.0f;

			++this->m_uiCharactersShown;

			if(this->m_uiCharactersShown == this->m_szDialogueText.size())
			{
				this->m_bDialogueFinishedPrinting = true;
			}
		}

		if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Tab)))
		{
			this->m_uiCharactersShown = this->m_szDialogueText.size();
			this->m_bDialogueFinishedPrinting = true;
		}
	}

	ImGui::Text(this->m_szCharacterName.c_str());
	
	ImGui::Separator();

	float const footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

	ImGui::BeginChild("ScrollingTextRegion", ImVec2(390, 300 - footer_height_to_reserve * 2), false);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

	std::string consoleText = this->m_szDialogueText.substr(0, this->m_uiCharactersShown);

	ImGui::TextWrapped(consoleText.c_str());

	if(this->m_bScrollToBottom || (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
	{
		ImGui::SetScrollHere(1.0f);
	}
	this->m_bScrollToBottom = false;

	ImGui::PopStyleVar();
	ImGui::EndChild();

	ImGui::Separator();

	ImGui::BeginChild("ResponsesRegion", ImVec2(390, 100 - footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

	if(this->m_bDialogueFinishedPrinting)
	{
		if(this->m_vDialoguePossibleResponses.size())
		{
			for(CDialogueResponse* pResponse : this->m_vDialoguePossibleResponses)
			{
				std::string szVariableKey;
				std::string szVariableValue;
				bool bSetsVariable = pResponse->get_variable_set(szVariableKey, szVariableValue);

				//ImVec4 responseColor = bSetsVariable ? ImColor(255, 255, 0) : ImColor(232, 232, 232);

				//ImGui::PushStyleColor(ImGuiCol_Text, responseColor);

				if(ImGui::Selectable(pResponse->get_response().c_str()))
				{
					if(bSetsVariable)
					{
						SG::get_intransient_data_manager()->set_string_variable(szVariableKey, szVariableValue);
					}

					this->m_uiDialogueLine = pResponse->get_next_dialogue();

					this->begin_dialogue();

					break;
				}

				//ImGui::PopStyleColor();
			}
		}
		else
		{
			if(ImGui::Selectable("(End Dialogue)"))
			{
				this->m_bPanelActive = false;
			}
		}
	}

	ImGui::EndChild();

	ImGui::End();
};

void DialoguePanel::begin_dialogue(void)
{
	CDialogueManager* pDialogueManager = SG::get_dialogue_manager();

	CDialogueLine *pDialogueLine = pDialogueManager->get_dialogue_line(this->m_uiDialogueLine);

	if(pDialogueLine == nullptr)
	{
		throw SGException("Invalid dialogue line");
	}

	std::vector<DialogueResponseId> vDialogueResponses;
	pDialogueLine->get_dialogue_possible_responses(vDialogueResponses);

	this->m_szDialogueText = pDialogueLine->get_dialogue_text();
	this->m_szCharacterName = pDialogueLine->get_character_name();

	this->m_vDialoguePossibleResponses.clear();
	
	for(DialogueResponseId responseId : vDialogueResponses)
	{
		CDialogueResponse* pDialogueResponse = pDialogueManager->get_dialogue_response(responseId);

		if(pDialogueResponse == nullptr)
		{
			throw SGException("Invalid dialogue response");
		}

		this->m_vDialoguePossibleResponses.push_back(pDialogueResponse);
	}

	this->m_flDialogueTimer = 0.0f;
	this->m_bDialogueInitialized = true;
	this->m_bDialogueFinishedPrinting = false;
	this->m_uiCharactersShown = 0;
}