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

#include <mutex>
#include <functional>
#include <unordered_map>
#include "IDebugConsole.hxx"

typedef std::unordered_map<std::string, ConsoleCommandCallback> ConsoleCommandMap;

class CDebugConsole : public IDebugConsole
{
public:
	CDebugConsole();
	~CDebugConsole();

	virtual void console_write_line(std::string const&);
	virtual void console_issue_command(std::string const&);
	virtual void console_get_latest_n_lines(int const, std::string&);
	virtual void console_clear(void);
	virtual void console_register_command(std::string const&, ConsoleCommandCallback const&);

private:
	std::mutex m_mFieldAccess;

	struct ConsoleLine
	{
		std::string m_szLineText;
		ConsoleLine* m_pListNext;
		ConsoleLine* m_pListPrev;
	};

	std::string m_szConsoleText;
	int m_uiConsoleTextLines;
	bool m_bConsoleUpdated;
	int m_uiLinesCount;
	ConsoleLine* m_pListHead;
	ConsoleLine* m_pListTail;

	ConsoleCommandMap m_commandMap;

	void clear();
	void write_line(std::string const &);
};