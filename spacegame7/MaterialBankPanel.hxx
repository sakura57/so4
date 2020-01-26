#pragma once

#include "InterfacePanel.hxx"
#include "IMaterialBank.hxx"

class MaterialBankPanel : public InterfacePanel
{
public:
	MaterialBankPanel(IMaterialBank* pMatBank)
		: m_pMatBank(pMatBank)
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
	virtual ~MaterialBankPanel()
	{
		this->m_bPanelExists = false;
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
	static bool m_bPanelExists;
	bool m_bPanelActive;
};