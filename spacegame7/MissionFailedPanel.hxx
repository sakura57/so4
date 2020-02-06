#pragma once

#include <future>

#include "InterfacePanel.hxx"

class MissionFailedPanel : public InterfacePanel
{
public:
	MissionFailedPanel(std::string const& szDeathString)
		: m_szDeathString(szDeathString)
	{
		if(++MissionFailedPanel::m_iPanelInstances > 1)
		{
			this->m_bPanelActive = false;
		}
		else
		{
			this->m_bPanelActive = true;
		}
	};
	virtual ~MissionFailedPanel()
	{
		--MissionFailedPanel::m_iPanelInstances;
	};

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void);

	virtual bool prevents_game_input(void)
	{
		return true;
	};

private:
	std::string m_szDeathString;
	bool m_bPanelActive;
	std::future<bool> m_bGameLoadAttempt;

	static int m_iPanelInstances;
};