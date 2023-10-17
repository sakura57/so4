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

typedef unsigned int DialogueLineId;
typedef unsigned int DialogueResponseId;

class CDialogueResponse
{
public:
	CDialogueResponse() = delete;
	CDialogueResponse(CDialogueResponse const&) = delete;
	CDialogueResponse& operator=(CDialogueResponse const&) = delete;

	CDialogueResponse(DialogueResponseId const, std::string const &, DialogueLineId const, std::string const &, std::string const &);
	~CDialogueResponse();

	DialogueResponseId get_dialogue_id(void) const;
	std::string get_response(void) const;
	DialogueLineId get_next_dialogue(void) const;

	bool get_variable_set(std::string &, std::string &) const;

private:
	DialogueResponseId m_uiDialogueId;
	std::string m_szResponse;
	DialogueLineId m_uiNextDialogue;
	std::string m_szVariableKey;
	std::string m_szVariableValue;
};