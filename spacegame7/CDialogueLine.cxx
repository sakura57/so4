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
#include "CDialogueLine.hxx"

CDialogueLine::CDialogueLine(
	DialogueLineId const uiDialogueId, 
	std::string const &szCharacterName,
	std::string const &szDialogueLine,
	std::vector<DialogueResponseId> &vDialoguePossibleResponses)
	: m_uiDialogueId(uiDialogueId),
	m_szCharacterName(szCharacterName),
	m_szDialogueText(szDialogueLine),
	m_vDialoguePossibleResponses(vDialoguePossibleResponses)
{
	
}

CDialogueLine::~CDialogueLine()
{
}

std::string CDialogueLine::get_character_name() const
{
	return this->m_szCharacterName;
}

std::string CDialogueLine::get_dialogue_text() const
{
	return this->m_szDialogueText;
}

void CDialogueLine::get_dialogue_possible_responses(std::vector<DialogueResponseId>& vDialoguePossibleResponses) const
{
	vDialoguePossibleResponses.clear();

	vDialoguePossibleResponses = this->m_vDialoguePossibleResponses;
}

DialogueLineId CDialogueLine::get_dialogue_id(void) const
{
	return this->m_uiDialogueId;
}