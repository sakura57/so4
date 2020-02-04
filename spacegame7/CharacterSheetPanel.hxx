#pragma once

#include "ICharacterEntity.hxx"
#include "InterfacePanel.hxx"

class CharacterSheetPanel : public InterfacePanel
{
public:
	CharacterSheetPanel(ICharacterEntity *pEntity)
		: m_pEntity(pEntity)
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
	virtual ~CharacterSheetPanel()
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
	ICharacterEntity *m_pEntity;
	bool m_bPanelActive;
	static int m_iPanelInstances;
};