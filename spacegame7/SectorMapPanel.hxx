#pragma once

#include <sstream>

#include "InterfacePanel.hxx"
#include "CSector.hxx"
#include "SGLib.hxx"

class SectorMapPanel : public InterfacePanel
{
public:
	SectorMapPanel(SectorId const);
	virtual ~SectorMapPanel();

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
	bool m_bPanelActive;
	static int m_iPanelInstances;
	SectorId m_uiSectorId;
	CSector const *m_pSector;
	ISectorMapRenderer* m_pSectorMapRenderer;
};