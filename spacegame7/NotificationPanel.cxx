/**
 * Copyright (C) 2020 Jacob Farnsworth.
 *
 * This file is part of the Spaced Out 4 project.
 *
 * Spaced Out 4 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, version 2.
 *
 * Spaced Out 4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "NotificationPanel.hxx"

#define NOTIFICATION_DURATION 2.5f
#define NOTIFICATION_SCROLL_SPEED 10.0f

void NotificationPanel::render_panel(float const flDelta)
{
	ImVec2 displaySize = ImGui::GetIO().DisplaySize;

	ImVec2 textSize = ImGui::CalcTextSize(this->m_szText.c_str());
	ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2.0f - textSize.x / 2.0f, displaySize.y / 3.75f + clamp(2.5f - this->m_flDuration, 0.0f, 1.0f) * NOTIFICATION_SCROLL_SPEED), ImGuiCond_Always);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::Begin(
		"Notification",
		nullptr,
		ImVec2(100.0f, 100.0f),
		0.0f,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoFocusOnAppearing
	);

	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, clamp(2.5f - this->m_flDuration, 0.0f, 1.0f)), this->m_szText.c_str());

	ImGui::End();

	ImGui::PopStyleVar();

	this->m_flDuration += flDelta;

	if(this->m_flDuration > NOTIFICATION_DURATION)
	{
		this->m_bActive = false;
	}
}