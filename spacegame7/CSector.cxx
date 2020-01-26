#include "CSector.hxx"

CSector::CSector()
	: m_szName("Invalid Sector"), m_szDescription("Invalid sector."), m_szScriptPath("data/scripts/systems/null_sector.lua"), m_iGoverningFaction(0)
{
	
}

CSector::CSector(std::string const &szName,
	std::string const &szDesc,
	std::string const &szScriptPath,
	FactionId const iGoverningFaction)
	: m_szName(szName), m_szDescription(szDesc), m_szScriptPath(szScriptPath), m_iGoverningFaction(iGoverningFaction)
{
}

CSector::~CSector()
{
	
}

std::string CSector::get_name(void) const
{
	return this->m_szName;
}

std::string CSector::get_desc(void) const
{
	return this->m_szDescription;
}

std::string CSector::get_script_path(void) const
{
	return this->m_szScriptPath;
}

FactionId CSector::get_governing_faction(void) const
{
	return this->m_iGoverningFaction;
}