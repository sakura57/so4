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