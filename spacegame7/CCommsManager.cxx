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
#include "CCommsManager.hxx"

void CCommsManager::send_comm_unimportant(std::string const & sender, std::string const & text)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(!this->m_lComms.size() && this->m_activeComm.uiPosition == -1)
	{
		this->m_lComms.emplace_back(CComm(sender, text));
	}
}

void CCommsManager::send_comm(std::string const & sender, std::string const & text)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	this->m_lComms.emplace_back(CComm(sender, text));
}

void CCommsManager::render_comms()
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if (m_activeComm.uiPosition == -1)
	{
		//no active comm, nothing to do
		return;
	}

	ImGui::Begin("Transmission", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
	ImGui::PushID(1);
	ImGui::Text(m_activeComm.szSender.c_str());
	ImGui::PopID();

	ImGui::Separator();

	ImGui::PushID(2);
	ImGui::TextWrapped(m_activeComm.szText.substr(0, m_activeComm.uiPosition).c_str());
	ImGui::PopID();

	ImGui::End();
}

void CCommsManager::tick_comms(float const flDelta)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(this->m_flBetweenTime > 0.0f)
	{
		this->m_flBetweenTime -= flDelta;
	}

	if(m_activeComm.uiPosition == -1)
	{
		if(!m_lComms.size() || m_flBetweenTime > 0.0f)
		{
			//no active comm, no pending comms, nothing to do
			return;
		}

		m_activeComm = m_lComms.front();
		m_lComms.pop_front();
	}

	m_activeComm.flTimeElapsed += flDelta;

	if(m_activeComm.flTimeElapsed > 7.5f)
	{
		m_activeComm.uiPosition = -1;
		m_flBetweenTime = 1.0f;
	}
	else if(m_activeComm.flTimeElapsed > 5.0f)
	{
		m_activeComm.uiPosition = m_activeComm.szText.size();
	}
	else
	{
		m_activeComm.uiPosition = (size_t)((m_activeComm.flTimeElapsed) / 5.0f * m_activeComm.szText.size());
	}
}

void CCommsManager::shifting_out(void)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	this->m_flBetweenTime = 0.0f;
	this->m_lComms.clear();
	this->m_activeComm = CComm();
}
