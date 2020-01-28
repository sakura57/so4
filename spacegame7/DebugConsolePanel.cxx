#include "DebugConsolePanel.hxx"
#include "SGLib.hxx"

#define INPUT_BUFFER_SIZE 256

bool DebugConsolePanel::m_bPanelExists = false;

DebugConsolePanel::DebugConsolePanel()
	: m_bScrollToBottom(false)
{
	if(this->m_bPanelExists)
	{
		this->m_bPanelActive = false;
	}
	else
	{
		this->m_bPanelActive = true;
		this->m_bPanelExists = true;
	}

	this->m_szInputBuffer = new char[INPUT_BUFFER_SIZE];

	memset(this->m_szInputBuffer, 0, INPUT_BUFFER_SIZE);
}

DebugConsolePanel::~DebugConsolePanel()
{
	this->m_bPanelExists = false;

	delete[] this->m_szInputBuffer;
}

void DebugConsolePanel::render_panel(float const flDelta)
{
	IDebugConsole* pDebugConsole = SG::get_debug_console();

	ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoFocusOnAppearing);

	ImGui::Separator();

	float const footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y * 2 + ImGui::GetFrameHeightWithSpacing() * 2;

	ImGui::BeginChild("ScrollingTextRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

	if(ImGui::BeginPopupContextWindow())
	{
		if(ImGui::Selectable("Clear"))
		{
			pDebugConsole->console_clear();
		}

		ImGui::EndPopup();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

	std::string consoleText;
	pDebugConsole->console_get_latest_n_lines(40, consoleText);

	ImGui::TextUnformatted(consoleText.c_str());

	if(this->m_bScrollToBottom || (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
	{
		ImGui::SetScrollHere(1.0f);
	}
	this->m_bScrollToBottom = false;

	ImGui::PopStyleVar();
	ImGui::EndChild();

	ImGui::Separator();

	bool reclaim_focus = false;
	if(ImGui::InputText("Input", this->m_szInputBuffer, INPUT_BUFFER_SIZE, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		if(this->m_szInputBuffer[0])
		{
			std::string szCommandExecuted(">");
			szCommandExecuted.append(this->m_szInputBuffer);

			pDebugConsole->console_write_line(szCommandExecuted);
			pDebugConsole->console_issue_command(std::string(this->m_szInputBuffer));

			this->m_bScrollToBottom = true;
		}

		strcpy_s(this->m_szInputBuffer, INPUT_BUFFER_SIZE, "");

		reclaim_focus = true;
	}

	ImGui::SetItemDefaultFocus();
	if(reclaim_focus)
	{
		ImGui::SetKeyboardFocusHere(-1);
	}

	if(ImGui::Button("Exit"))
	{
		this->m_bPanelExists = false;
		this->m_bPanelActive = false;
	}

	ImGui::End();
};