#pragma once

#include "InterfacePanel.hxx"
#include "SGLib.hxx"
#include "CMainMenuState.hxx"

class InGameMenuPanel : public InterfacePanel
{
public:
	InGameMenuPanel()
	{};
	virtual ~InGameMenuPanel()
	{};

	virtual void render_panel(float const flDelta)
	{
		ImGui::SetNextWindowPosCenter(ImGuiCond_Once | ImGuiCond_Appearing);

		ImGui::Begin(
			"In-Game Menu",
			nullptr,
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse
		);

		if (ImGui::Button("Exit to Menu"))
		{
			SG::get_game_state_manager()->transition_game_state(new CMainMenuState);
		}

		ImGui::Button("Exit to Desktop");

		ImGui::End();
	};

	virtual bool panel_active(void)
	{
		return true;
	};

	virtual bool prevents_game_input(void)
	{
		return true;
	};
};