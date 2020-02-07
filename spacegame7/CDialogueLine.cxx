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