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
#pragma once

#include <string>
#include <list>
#include <mutex>

#include "ITransientStateStructure.hxx"
#include "imgui.h"

#define DEFAULT_COMM_TIME 10.0f

struct CComm
{
	CComm(std::string const &sender, std::string const &text)
		: szSender(sender), szText(text), uiPosition(0), flTimeElapsed(0.0f)
	{};
	CComm()
		: szSender(), szText(), uiPosition(-1), flTimeElapsed(0.0f)
	{};
	~CComm() {};

	size_t uiPosition;
	float flTimeElapsed;
	std::string szSender;
	std::string szText;
};

class CCommsManager : public ITransientStateStructure
{
public:
	CCommsManager() {};
	~CCommsManager() {};

	/*
	 * Send a comm. This will always be placed in the queue.
	 */
	void send_comm(std::string const &sender, std::string const &text);

	/*
	 * If there is at least one pending comm or an active comm,
	 * the sent comm will be ignored.
	 */
	void send_comm_unimportant(std::string const &sender, std::string const &text);

	void render_comms(void);

	void tick_comms(float const flDelta);

	//from ITransientStateStructure
	virtual void shifting_out(void) final;

private:
	std::mutex m_mFieldAccess;

	float m_flBetweenTime;
	std::list<CComm> m_lComms;
	CComm m_activeComm;
};