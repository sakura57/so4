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

#include "InterfacePanel.hxx"
#include "CDialogueResponse.hxx"
#include "CDialogueLine.hxx"

class DialoguePanel : public InterfacePanel
{
public:
	DialoguePanel(DialogueLineId const);
	virtual ~DialoguePanel();

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void)
	{
		return this->m_bPanelActive;
	};

	virtual bool prevents_game_input(void)
	{
		return true;
	};

	void begin_dialogue(void);

private:
	bool m_bPanelActive;
	bool m_bScrollToBottom;
	bool m_bDialogueInitialized;
	bool m_bDialogueFinishedPrinting;
	DialogueLineId m_uiDialogueLine;
	std::vector<CDialogueResponse *> m_vDialoguePossibleResponses;
	unsigned int m_uiCharactersShown;
	std::string m_szDialogueText;
	std::string m_szCharacterName;
	static int m_iPanelInstances;
	float m_flDialogueTimer;
};