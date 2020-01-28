#pragma once

#include <string>
#include "Defs.hxx"

/*
 * Abstract class representing the debug console
 */
interface IDebugConsole
{
	virtual void console_write_line(std::string const &) = 0;
	virtual void console_issue_command(std::string const &) = 0;
	virtual void console_get_latest_n_lines(int const, std::string &) = 0;
	virtual void console_clear(void) = 0;
};