#pragma once

#include "InterfacePanel.hxx"
#include "SGLib.hxx"
#include "CInSpaceState.hxx"
#include "CSectorTransitionState.hxx"
#include "CharacterCreationPanel.hxx"
#include "SettingsPanel.hxx"
#include "ArenaStartPanel.hxx"
#include "LoadGamePanel.hxx"

class MainMenuPanel : public InterfacePanel
{
public:
	MainMenuPanel()
		: m_bPanelActive(true)
	{};
	virtual ~MainMenuPanel()
	{};

	virtual void render_panel(float const);

	virtual bool panel_active(void)
	{
		return m_bPanelActive;
	};

	virtual bool prevents_game_input(void)
	{
		return false;
	};
private:
	bool m_bPanelActive;
};