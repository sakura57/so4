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
#include "CDialogueResponse.hxx"

CDialogueResponse::CDialogueResponse(
	DialogueResponseId const myId,
	std::string const &szResponse,
	DialogueLineId const nextDialogue,
	std::string const &szVariableKey,
	std::string const &szVariableValue
)
	: m_uiDialogueId(myId),
	m_szResponse(szResponse),
	m_uiNextDialogue(nextDialogue),
	m_szVariableKey(szVariableKey),
	m_szVariableValue(szVariableValue)
{
}

CDialogueResponse::~CDialogueResponse()
{

}

std::string CDialogueResponse::get_response() const
{
	return this->m_szResponse;
}

DialogueLineId CDialogueResponse::get_next_dialogue() const
{
	return this->m_uiNextDialogue;
}

DialogueResponseId CDialogueResponse::get_dialogue_id() const
{
	return this->m_uiDialogueId;
}

bool CDialogueResponse::get_variable_set(std::string &szKey, std::string &szValue) const
{
	if(this->m_szVariableKey.size())
	{
		szKey = this->m_szVariableKey;
		szValue = this->m_szVariableValue;

		return true;
	}
	else
	{
		return false;
	}
}