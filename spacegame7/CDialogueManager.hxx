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