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