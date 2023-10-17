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

#include <mutex>
#include <vector>

#include "Defs.hxx"
#include "CDialogueLine.hxx"

class CDialogueManager
{
public:
	CDialogueManager();
	~CDialogueManager();

	void add_dialogue_line(DialogueLineId const, CDialogueLine*);
	CDialogueLine* get_dialogue_line(DialogueLineId const);
	void add_dialogue_response(DialogueResponseId const, CDialogueResponse*);
	CDialogueResponse* get_dialogue_response(DialogueResponseId const);

private:
	std::mutex m_mFieldAccess;

	std::vector<CDialogueLine*> m_vDialogueLines;
	std::vector<CDialogueResponse*> m_vDialogueResponses;
};