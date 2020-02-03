#pragma once

#include <future>

#include "InterfacePanel.hxx"

class PlayerDiedPanel : public InterfacePanel
{
public:
	PlayerDiedPanel(std::string const &szDeathString)
		: m_szDeathString(szDeathString)
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
	virtual ~PlayerDiedPanel()
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
	std::string m_szDeathString;
	static bool m_bPanelExists;
	bool m_bPanelActive;
	std::future<bool> m_bGameLoadAttempt;
};