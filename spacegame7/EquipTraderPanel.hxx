#pragma once

#include "SGLib.hxx"
#include "InterfacePanel.hxx"
#include "IEntityInventory.hxx"
#include "IUniverse.hxx"

class EquipTraderPanel : public InterfacePanel
{
public:
	EquipTraderPanel(BaseId const baseId)
		: m_iBaseId(baseId)
	{
		if(++this->m_iPanelInstances > 1)
		{
			this->m_bPanelActive = false;
		}
		else
		{
			this->m_bPanelActive = true;
		}

		this->m_pInventory = SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity()->get_inventory();
		this->m_vEquipmentSelling = SG::get_universe()->get_base(baseId).get_equipment_selling();
	};
	virtual ~EquipTraderPanel()
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
	char **m_pszItems;
	IEntityInventory *m_pInventory;
	BaseId m_iBaseId;
	std::vector<ArchId> m_vEquipmentSelling;
	bool m_bPanelActive;
	static int m_iPanelInstances;
};