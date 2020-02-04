#pragma once

#include "InterfacePanel.hxx"
#include "IMaterialBank.hxx"

class MaterialBankPanel : public InterfacePanel
{
public:
	MaterialBankPanel(IMaterialBank* pMatBank)
		: m_pMatBank(pMatBank)
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
	virtual ~MaterialBankPanel()
	{
		--this->m_iPanelInstances;
	};

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void);

	virtual bool prevents_game_input(void)
	{
		return true;
	};

	void do_mat_list(MaterialQuantityMap&);

private:
	char** m_pszItems;
	IMaterialBank* m_pMatBank;
	static int m_iPanelInstances;
	bool m_bPanelActive;
};