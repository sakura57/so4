#pragma once

#include "InterfacePanel.hxx"

class LoadGamePanel : public InterfacePanel
{
public:
	LoadGamePanel();

	virtual ~LoadGamePanel();

	virtual void render_panel(float const flDelta);

	virtual bool panel_active(void);

	virtual bool prevents_game_input(void)
	{
		return true;
	};

private:
	static int m_iPanelInstances;
	bool m_bPanelActive;
	bool m_bLoadingOperationInitiated;
	std::future<bool> m_fLoadingOperationResult;
};