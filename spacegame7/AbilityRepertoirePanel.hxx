#pragma once

#include <sstream>

#include "InterfacePanel.hxx"
#include "ICharacterEntity.hxx"

class AbilityRepertoirePanel : public InterfacePanel
{
public:
	AbilityRepertoirePanel(ICharacterEntity *pCharEntity)
		: m_pCharEntity(pCharEntity)
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
	virtual ~AbilityRepertoirePanel()
	{
		--this->m_iPanelInstances;
	};

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void)
	{
		return this->m_bPanelActive;
	};

	virtual bool prevents_game_input(void)
	{
		return true;
	};

private:
	ICharacterEntity *m_pCharEntity;
	bool m_bPanelActive;
	static int m_iPanelInstances;
};