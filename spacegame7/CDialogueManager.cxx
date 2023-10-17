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
#include "Util.hxx"
#include "CDialogueManager.hxx"

CDialogueManager::CDialogueManager()
{

}

CDialogueManager::~CDialogueManager()
{
	for(CDialogueResponse* pResponse : this->m_vDialogueResponses)
	{
		if(pResponse != nullptr)
		{
			delete pResponse;
		}
	}

	this->m_vDialogueResponses.clear();

	for(CDialogueLine* pLine : this->m_vDialogueLines)
	{
		if(pLine != nullptr)
		{
			delete pLine;
		}
	}

	this->m_vDialogueLines.clear();
}

CDialogueLine* CDialogueManager::get_dialogue_line(DialogueLineId const uiDialogueId)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(uiDialogueId >= this->m_vDialogueLines.size())
	{
		return nullptr;
	}

	return this->m_vDialogueLines[uiDialogueId];
}

void CDialogueManager::add_dialogue_line(DialogueLineId const uiDialogueId, CDialogueLine* pDialogueLine)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(this->m_vDialogueLines.size() <= uiDialogueId)
	{
		this->m_vDialogueLines.resize(uiDialogueId + 1);
	}

	this->m_vDialogueLines[uiDialogueId] = pDialogueLine;
}

CDialogueResponse* CDialogueManager::get_dialogue_response(DialogueResponseId const uiDialogueId)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(uiDialogueId >= this->m_vDialogueResponses.size())
	{
		return nullptr;
	}

	return this->m_vDialogueResponses[uiDialogueId];
}

void CDialogueManager::add_dialogue_response(DialogueResponseId const uiDialogueId, CDialogueResponse* pDialogueResponse)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(this->m_vDialogueResponses.size() <= uiDialogueId)
	{
		this->m_vDialogueResponses.resize(uiDialogueId + 1);
	}

	this->m_vDialogueResponses[uiDialogueId] = pDialogueResponse;
}