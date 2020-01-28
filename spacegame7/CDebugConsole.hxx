#pragma once

#include <mutex>
#include "IDebugConsole.hxx"

class CDebugConsole : public IDebugConsole
{
public:
	CDebugConsole();
	~CDebugConsole();

	virtual void console_write_line(std::string const&);
	virtual void console_issue_command(std::string const&);
	virtual void console_get_latest_n_lines(int const, std::string&);
	virtual void console_clear(void);

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

	void clear();
	void write_line(std::string const &);
};