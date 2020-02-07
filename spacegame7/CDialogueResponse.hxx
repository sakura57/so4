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