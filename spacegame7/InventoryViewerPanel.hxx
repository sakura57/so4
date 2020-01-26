#pragma once

#include "InterfacePanel.hxx"
#include "IEntityInventory.hxx"

class InventoryViewerPanel : public InterfacePanel
{
public:
	InventoryViewerPanel(IEntityInventory *pInventory)
		: m_pInventory(pInventory)
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
	virtual ~InventoryViewerPanel()
	{
		this->m_bPanelExists = false;
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
	static bool m_bPanelExists;
	bool m_bPanelActive;
};