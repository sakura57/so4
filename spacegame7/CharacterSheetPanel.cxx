#include "CharacterSheetPanel.hxx"

int CharacterSheetPanel::m_iPanelInstances = 0;

void CharacterSheetPanel::render_panel(float const flDelta)
{
	if(this->m_pEntity == nullptr)
	{
		return;
	}

	ImGui::Begin("Character");

	std::string szName = this->m_pEntity->get_name();
	int iCurrentLevel = this->m_pEntity->get_level();
	unsigned long ulCurrentExp = this->m_pEntity->get_current_exp();
	unsigned long ulExpRequired = this->m_pEntity->get_exp_to_next_level();
	unsigned long ulExpRemaining = ulExpRequired - ulCurrentExp;

	ImGui::Text("Name: %s", szName.c_str());

	ImGui::Text("Elite Pilot, Level %d", iCurrentLevel);

	if(this->m_pEntity->get_level() == 400)
	{
		ImGui::Text("Max Level (congrats, now go get a life)");
	}
	else
	{
		ImGui::Text("Current EXP: %lu", ulCurrentExp);
		ImGui::Text("EXP required to achieve level %d: %lu", iCurrentLevel + 1, ulExpRequired);
		ImGui::Text("EXP remaining: %lu", ulExpRemaining);
	}

	if(ImGui::Button("Done"))
	{
		this->m_bPanelActive = false;
	}

	ImGui::End();
}

bool CharacterSheetPanel::panel_active(void)
{
	return this->m_bPanelActive;
}