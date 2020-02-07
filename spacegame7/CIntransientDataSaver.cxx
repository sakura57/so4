#include "CIntransientDataSaver.hxx"
#include "ini_parser.h"
#include "CGameDataManager.hxx"
#include "SGLib.hxx"

#define LOADER_MAX_SECTIONS_BUFFER_SIZE 8192
#define LOADER_MAX_VALUE_BUFFER_SIZE 2048

CIntransientDataSaver::CIntransientDataSaver(IIntransientDataManager* pIDM)
{
	this->m_pIDM = pIDM;
}

void CIntransientDataSaver::dump_to_save(std::string const& saveFile)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szStringBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];
	char const* szSaveFile = saveFile.c_str();

	bool bFoundHeaderSection = false;
	bool bFirst = true;

	IIntransientDataManager* pIDM = this->m_pIDM;

	std::string variables = pIDM->dump_string_variables();

	ReadWriteIniKeyValueStringA("Game", "globals", const_cast<char*>(variables.c_str()), saveFile.c_str(), true);
}

void CIntransientDataSaver::load_from_save(std::string const& saveFile)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szStringBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];
	char const* szSaveFile = saveFile.c_str();

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, szSaveFile, false, szSectionNamesBuffer, &uiSectionNamesLen);

	char* szSection = szSectionNamesBuffer;

	if(strlen(szSection) == 0)
	{
		throw SGException("save file doesn\'t exist, or contains no sections");
	}

	IIntransientDataManager* pIDM = this->m_pIDM;

	std::string s;
	std::stringstream ss;

	pIDM->clear_string_variables();

	while(*szSection)
	{
		unsigned int uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		if(!stricmp(szSection, "Game"))
		{
			uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(szSaveFile, szSection, "globals", "", szStringBuffer, uiStringLen);

			std::string key;
			std::string value;

			ss.clear();
			ss.str(szStringBuffer);
			while(ss >> key && ss >> value)
			{
				pIDM->set_string_variable(key, value);
			}
		}

		szSection += strlen(szSection) + 1;
	}
}