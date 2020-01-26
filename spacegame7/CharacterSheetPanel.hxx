#pragma once

#include "ICharacterEntity.hxx"
#include "InterfacePanel.hxx"

class CharacterSheetPanel : public InterfacePanel
{
public:
	CharacterSheetPanel(ICharacterEntity *pEntity)
		: m_pEntity(pEntity)
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
	virtual ~CharacterSheetPanel()
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
	ICharacterEntity *m_pEntity;
	bool m_bPanelActive;
	static bool m_bPanelExists;
};