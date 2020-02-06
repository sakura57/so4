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