#include "CDialogueResponse.hxx"

CDialogueResponse::CDialogueResponse(
	DialogueResponseId const myId,
	std::string const &szResponse,
	DialogueLineId const nextDialogue,
	std::string const &szVariableKey,
	std::string const &szVariableValue
)
	: m_uiDialogueId(myId),
	m_szResponse(szResponse),
	m_uiNextDialogue(nextDialogue),
	m_szVariableKey(szVariableKey),
	m_szVariableValue(szVariableValue)
{
}

CDialogueResponse::~CDialogueResponse()
{

}

std::string CDialogueResponse::get_response() const
{
	return this->m_szResponse;
}

DialogueLineId CDialogueResponse::get_next_dialogue() const
{
	return this->m_uiNextDialogue;
}

DialogueResponseId CDialogueResponse::get_dialogue_id() const
{
	return this->m_uiDialogueId;
}

bool CDialogueResponse::get_variable_set(std::string &szKey, std::string &szValue) const
{
	if(this->m_szVariableKey.size())
	{
		szKey = this->m_szVariableKey;
		szValue = this->m_szVariableValue;

		return true;
	}
	else
	{
		return false;
	}
}