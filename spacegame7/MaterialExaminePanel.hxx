#pragma once

#include "InterfacePanel.hxx"
#include "IEntityInventory.hxx"

class MaterialExaminePanel : public InterfacePanel
{
public:
	MaterialExaminePanel(std::string szMaterialSerial)
		: m_szMaterialSerial(szMaterialSerial)
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

		this->build_id_text_block();
		this->build_attribs_text_block();
	};
	virtual ~MaterialExaminePanel()
	{
		this->m_bPanelExists = false;
	};

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void);

	virtual bool prevents_game_input(void)
	{
		return true;
	};

private:
	std::string m_szMaterialSerial;
	std::string m_szIdText;
	std::string m_szMatAttribsText;
	static bool m_bPanelExists;
	bool m_bPanelActive;

	void build_id_text_block(void);
	void build_attribs_text_block(void);
};