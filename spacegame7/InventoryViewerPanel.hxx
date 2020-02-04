#pragma once

#include "InterfacePanel.hxx"
#include "IEntityInventory.hxx"

class InventoryViewerPanel : public InterfacePanel
{
public:
	InventoryViewerPanel(IEntityInventory *pInventory)
		: m_pInventory(pInventory)
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
	virtual ~InventoryViewerPanel()
	{
		--this->m_iPanelInstances;
	};

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void);

	static void do_item_list(ItemList &);

	virtual bool prevents_game_input(void)
	{
		return true;
	};

private:
	char **m_pszItems;
	IEntityInventory *m_pInventory;
	static int m_iPanelInstances;
	bool m_bPanelActive;
};