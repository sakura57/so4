#pragma once

#include "InterfacePanel.hxx"
#include "CAstfield.hxx"
#include <unordered_map>
#include <string>

#define BUFFER_SIZE 512

typedef std::unordered_map<std::string, unsigned int> MaterialQuantityMap;

class AsteroidProspectPanel : public InterfacePanel
{
public:
	AsteroidProspectPanel(AstfieldId uiAstfieldId)
		: m_uiAstfieldId(uiAstfieldId), m_szDepositSerial("")
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
		this->build_desc_text_block();
	};
	virtual ~AsteroidProspectPanel()
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
	AstfieldId m_uiAstfieldId;
	std::string m_szIdText;
	std::string m_szDescText;
	std::string m_szDepositSerial;
	static int m_iPanelInstances;
	bool m_bPanelActive;
	char m_szDescBuffer[BUFFER_SIZE];

	MaterialQuantityMap m_matQuantityMap;

	void build_id_text_block(void);
	void build_desc_text_block(void);

	static void do_mat_list(MaterialQuantityMap&);
};