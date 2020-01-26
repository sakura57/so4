#pragma once

#include <sstream>

#include "InterfacePanel.hxx"
#include "ICharacterEntity.hxx"

class AbilityRepertoirePanel : public InterfacePanel
{
public:
	AbilityRepertoirePanel(ICharacterEntity *pCharEntity)
		: m_pCharEntity(pCharEntity)
	{
		if(this->m_bPanelExists)
		{
			this->m_bPanelActive = false;
		}
		else
		{
			this->m_bPanelActive = true;
			this->m_bPanelExists = true;
		}
	};
	virtual ~AbilityRepertoirePanel()
	{
		this->m_bPanelExists = false;
	};

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void)
	{
		return this->m_bPanelActive;
	};

	virtual bool prevents_game_input(void)
	{
		return true;
	};

private:
	ICharacterEntity *m_pCharEntity;
	bool m_bPanelActive;
	static bool m_bPanelExists;
};