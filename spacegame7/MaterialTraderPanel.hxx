#pragma once

#include "SGLib.hxx"
#include "InterfacePanel.hxx"
#include "IEntityInventory.hxx"
#include "IUniverse.hxx"

class MaterialTraderPanel : public InterfacePanel
{
public:
	MaterialTraderPanel(BaseId const baseId)
		: m_iBaseId(baseId), m_pSelectedMaterial(nullptr)
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
		this->m_pMaterialBank = SG::get_intransient_data_manager()->get_character_entity_manager()->get_player_character_entity()->get_material_bank();
	};
	virtual ~MaterialTraderPanel()
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
	void do_mat_list(MaterialQuantityMap&);

	char** m_pszItems;
	IEntityInventory* m_pInventory;
	IMaterialBank* m_pMaterialBank;
	IMaterial* m_pSelectedMaterial;
	BaseId m_iBaseId;
	int m_iExchangeRate;
	bool m_bPanelActive;
	static int m_iPanelInstances;
};