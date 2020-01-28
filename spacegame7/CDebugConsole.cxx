#include "CDebugConsole.hxx"
#include "Util.hxx"

#define CONSOLE_MAXIMUM_LINES 80

CDebugConsole::CDebugConsole()
	: m_uiLinesCount(0), m_pListHead(nullptr), m_pListTail(nullptr), m_bConsoleUpdated(false), m_uiConsoleTextLines(0), m_szConsoleText("")
{

}

CDebugConsole::~CDebugConsole()
{
	this->clear();
}

void CDebugConsole::console_write_line(std::string const &text)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->write_line(text);
}

void CDebugConsole::console_issue_command(std::string const &text)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	//TODO: implement
}

void CDebugConsole::console_get_latest_n_lines(int const lines, std::string &text)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(lines == this->m_uiConsoleTextLines && this->m_bConsoleUpdated == false)
	{
		text = this->m_szConsoleText;
	}
	else
	{
		std::string consoleText("");

		ConsoleLine* pLine = this->m_pListTail;

		for(int i = 0; i < lines; ++i)
		{
			std::string lineText;

			if(pLine == nullptr)
			{
				break;
			}

			lineText = pLine->m_szLineText;

			if(i == 0)
			{
				consoleText = lineText;
			}
			else
			{
				consoleText = lineText + "\n" + consoleText;
			}
			
			if(pLine)
			{
				pLine = pLine->m_pListPrev;
			}
		}

		//cache the console text in the event of a future request with the same line count
		this->m_szConsoleText = consoleText;
		this->m_uiConsoleTextLines = lines;
		this->m_bConsoleUpdated = false;

		text = consoleText;
	}
}

void CDebugConsole::console_clear()
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->clear();
}

void CDebugConsole::clear()
{
	ConsoleLine* pLine = this->m_pListHead;

	while(pLine != nullptr)
	{
		ConsoleLine* pLineNext = pLine->m_pListNext;

		delete pLine;

		pLine = pLineNext;
	}
}

void CDebugConsole::write_line(std::string const &text)
{
	ConsoleLine* pLine = new ConsoleLine;

	pLine->m_szLineText = text;

	if(this->m_pListHead == nullptr && this->m_pListTail == nullptr)
	{
		pLine->m_pListNext = nullptr;
		pLine->m_pListPrev = nullptr;

		this->m_pListHead = pLine;
		this->m_pListTail = pLine;
	}
	else
	{
		pLine->m_pListNext = nullptr;
		pLine->m_pListPrev = this->m_pListTail;

		this->m_pListTail->m_pListNext = pLine;
		this->m_pListTail = pLine;
	}

	//If we are over the maximum lines, "trim" the list by deleting the head, aka the oldest line
	if(this->m_uiLinesCount > CONSOLE_MAXIMUM_LINES)
	{
		ConsoleLine* pOldHead = this->m_pListHead;

		this->m_pListHead = pOldHead->m_pListNext;

		delete pOldHead;
	}
	else
	{
		++this->m_uiLinesCount;
	}

	this->m_bConsoleUpdated = true;
}