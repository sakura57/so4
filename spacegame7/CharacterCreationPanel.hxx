#pragma once

#include "InterfacePanel.hxx"

class CharacterCreationPanel : public InterfacePanel
{
public:
	CharacterCreationPanel()
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
	virtual ~CharacterCreationPanel()
	{
		this->m_bPanelExists = false;
	};

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void);

	virtual bool prevents_game_input(void)
	{
		return false;
	};

private:
	static bool m_bPanelExists;
	bool m_bPanelActive;
};