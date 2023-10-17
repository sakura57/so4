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
#include "InterfaceManager.hxx"
#include "SGLib.hxx"

InterfaceManager::InterfaceManager()
{
	/*ImGuiIO &io = ImGui::GetIO();

	ImFontConfig fontConfig;

	//fontConfig.OversampleH = 1;
	//fontConfig.OversampleV = 1;
	fontConfig.MergeMode = false;
	fontConfig.PixelSnapH = false;
	fontConfig.FontDataOwnedByAtlas = true;

	this->m_pFont = io.Fonts->AddFontFromFileTTF("Gputeks-Regular.ttf", 18.0f, &fontConfig, io.Fonts->GetGlyphRangesDefault());
	*/
	ImGui::StyleColorsDark();

	ImGuiStyle &style = ImGui::GetStyle();

	style.WindowBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.FrameRounding = 4.0f;
	style.ChildRounding = 4.0f;
	style.WindowRounding = 4.0f;
	style.ButtonTextAlign = ImVec2(0.5f, 0.0f);

	this->m_pAudioManager = SG::get_audio_manager();

	this->m_iInputHinderingPanelCount = 0;
}

InterfaceManager::~InterfaceManager()
{
}

void InterfaceManager::render_all_panels(float const flDelta)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	ImGui::SetWindowFocus();

	this->m_mQueueAccess.lock();

	if(this->m_vPendingPanels.size())
	{
		this->m_vPanels.insert(this->m_vPanels.end(), this->m_vPendingPanels.begin(), this->m_vPendingPanels.end());
		this->m_vPendingPanels.clear();
	}

	this->m_mQueueAccess.unlock();

	auto i = this->m_vPanels.begin();

	while(i != this->m_vPanels.end())
	{
		if(!(*i)->panel_active())
		{
			if((*i)->prevents_game_input())
			{
				--this->m_iInputHinderingPanelCount;
			}

			delete* i;

			this->m_vPanels.erase(i++);
		}
		else
		{
			(*(i++))->render_panel(flDelta);
		}
	}
}

void InterfaceManager::free_all_panels(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	for(InterfacePanel *pPanel : this->m_vPanels)
	{
		if(pPanel->prevents_game_input())
		{
			--this->m_iInputHinderingPanelCount;
		}

		delete pPanel;
	}

	this->m_vPanels.clear();
}

void InterfaceManager::add_panel(InterfacePanel *pPanel)
{
	SCOPE_LOCK(this->m_mQueueAccess);

	this->m_vPendingPanels.push_back(pPanel);

	if(pPanel->prevents_game_input())
	{
		++this->m_iInputHinderingPanelCount;
	}
}

void InterfaceManager::shifting_out(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	for (InterfacePanel *pPanel : this->m_vPanels)
	{
		if(pPanel->prevents_game_input())
		{
			--this->m_iInputHinderingPanelCount;
		}

		delete pPanel;
	}

	this->m_vPanels.clear();
}

size_t InterfaceManager::get_num_panels(void)
{
	this->m_mFieldAccess.lock();
	size_t size = this->m_vPanels.size();
	this->m_mFieldAccess.unlock();

	this->m_mQueueAccess.lock();
	size += this->m_vPendingPanels.size();
	this->m_mQueueAccess.unlock();

	return size;
}

void InterfaceManager::register_tangible_ui_event(void)
{
	this->m_pAudioManager->play_sound(18);
}

bool InterfaceManager::input_blocked(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);
	
	return this->m_iInputHinderingPanelCount > 0;
}