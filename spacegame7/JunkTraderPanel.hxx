#pragma once

#include "SGLib.hxx"
#include "InterfacePanel.hxx"
#include "IEntityInventory.hxx"
#include "IUniverse.hxx"

class JunkTraderPanel : public InterfacePanel
{
public:
	JunkTraderPanel(BaseId const baseId)
		: m_iBaseId(baseId)
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

		this->m_pInventory = SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity()->get_inventory();
		this->m_iExchangeRate = SG::get_universe()->get_base(baseId).get_metal_exchange();
	};
	virtual ~JunkTraderPanel()
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
	char **m_pszItems;
	IEntityInventory *m_pInventory;
	BaseId m_iBaseId;
	int m_iExchangeRate;
	bool m_bPanelActive;
	static bool m_bPanelExists;
};