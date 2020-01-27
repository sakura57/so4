#pragma once

#include <sstream>

#include "InterfacePanel.hxx"
#include "CSector.hxx"
#include "SGLib.hxx"

class SectorMapPanel : public InterfacePanel
{
public:
	SectorMapPanel(SectorId const uiSector)
		: m_uiSectorId(uiSector), m_pSector(nullptr), m_pSectorMapRenderer(SG::get_render_pipeline()->get_sector_map_renderer())
	{
		if(this->m_bPanelExists)
		{
			this->m_bPanelActive = false;
		}
		else
		{
			this->m_bPanelActive = true;
			this->m_bPanelExists = true;

			if(uiSector)
			{
				m_pSector = &SG::get_universe()->get_sector(uiSector);
			}
		}
	};
	virtual ~SectorMapPanel()
	{
		this->m_bPanelExists = false;
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
	bool m_bPanelActive;
	static bool m_bPanelExists;
	SectorId m_uiSectorId;
	CSector const *m_pSector;
	ISectorMapRenderer* m_pSectorMapRenderer;
};