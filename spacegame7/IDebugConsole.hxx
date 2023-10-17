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
#include <vector>
#include <functional>
#include "Defs.hxx"

typedef std::function<void(std::vector<std::string> const&)> ConsoleCommandCallback;

/*
 * Abstract class representing the debug console
 */
interface IDebugConsole
{
	virtual void console_write_line(std::string const &) = 0;
	virtual void console_issue_command(std::string const &) = 0;
	virtual void console_get_latest_n_lines(int const, std::string &) = 0;
	virtual void console_clear(void) = 0;
	virtual void console_register_command(std::string const &, ConsoleCommandCallback const &) = 0;
};