#pragma once

#include "InterfacePanel.hxx"
#include "IEntityInventory.hxx"

class MaterialExaminePanel : public InterfacePanel
{
public:
	MaterialExaminePanel(std::string szMaterialSerial)
		: m_szMaterialSerial(szMaterialSerial)
	{
		if(++this->m_iPanelInstances > 1)
		{
			this->m_bPanelActive = false;
		}
		else
		{
			this->m_bPanelActive = true;
		}

		this->build_id_text_block();
		this->build_attribs_text_block();
	};
	virtual ~MaterialExaminePanel()
	{
		--this->m_iPanelInstances;
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
	static int m_iPanelInstances;
	bool m_bPanelActive;

	void build_id_text_block(void);
	void build_attribs_text_block(void);
};