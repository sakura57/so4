#pragma once

#include "InterfacePanel.hxx"

class ArenaStartPanel : public InterfacePanel
{
public:
	ArenaStartPanel()
	{
		if(++this->m_iPanelInstances > 1)
		{
			this->m_bPanelActive = false;
		}
		else
		{
			this->m_bPanelActive = true;
		}
	};
	virtual ~ArenaStartPanel()
	{
		--this->m_iPanelInstances;
	};

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void);

	virtual bool prevents_game_input(void)
	{
		return false;
	};

private:
	static int m_iPanelInstances;
	bool m_bPanelActive;
};