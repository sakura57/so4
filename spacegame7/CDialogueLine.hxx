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
#pragma once

#include <vector>
#include <string>

#include "Defs.hxx"

#include "CDialogueResponse.hxx"

class CDialogueLine
{
public:
	CDialogueLine(CDialogueLine const&) = delete;
	CDialogueLine& operator=(CDialogueLine const&) = delete;
	CDialogueLine(DialogueLineId const, std::string const &, std::string const &, std::vector<DialogueResponseId> &);
	~CDialogueLine();

	DialogueLineId get_dialogue_id(void) const;
	std::string get_character_name(void) const;
	std::string get_dialogue_text(void) const;
	void get_dialogue_possible_responses(std::vector<DialogueResponseId> &) const;

private:
	DialogueLineId m_uiDialogueId;
	std::string m_szCharacterName;
	std::string m_szDialogueText;
	std::vector<DialogueResponseId> m_vDialoguePossibleResponses;
};