#pragma once

#include "SGLib.hxx"
#include "InterfacePanel.hxx"
#include "IEntityInventory.hxx"
#include "IUniverse.hxx"

class SaveGamePanel : public InterfacePanel
{
public:
	SaveGamePanel()
	{
		if(++SaveGamePanel::m_iPanelInstances > 1)
		{
			this->m_bPanelActive = false;
		}
		else
		{
			this->m_bPanelActive = true;
		}
	};
	virtual ~SaveGamePanel()
	{
		--SaveGamePanel::m_iPanelInstances;
	};

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void);

	virtual bool prevents_game_input(void)
	{
		return true;
	};

private:
	bool m_bPanelActive;
	static int m_iPanelInstances;
};