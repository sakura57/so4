#pragma once

#include "InterfacePanel.hxx"

class LoadGamePanel : public InterfacePanel
{
public:
	LoadGamePanel()
	{
		if (this->m_bPanelExists)
		{
			this->m_bPanelActive = false;
		}
		else
		{
			this->m_bPanelActive = true;
			this->m_bPanelExists = true;
		}
	};
	virtual ~LoadGamePanel()
	{
		this->m_bPanelExists = false;
	};

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void);

	virtual bool prevents_game_input(void)
	{
		return true;
	};

private:
	static bool m_bPanelExists;
	bool m_bPanelActive;
};