#pragma once

#include "InterfacePanel.hxx"
#include "SGLib.hxx"
#include "CMainMenuState.hxx"

class NotificationPanel : public InterfacePanel
{
public:
	NotificationPanel(std::string const &szText)
		: m_bActive(true), m_szText(szText), m_flDuration(0.0f)
	{};
	virtual ~NotificationPanel()
	{};

	virtual void render_panel(float const);

	virtual bool panel_active(void)
	{
		return this->m_bActive;
	};

	virtual bool prevents_game_input(void)
	{
		return false;
	};

private:
	std::string m_szText;
	float m_flDuration;
	bool m_bActive;
};