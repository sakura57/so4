#include "CSector.hxx"

CSector::CSector()
	: m_szName("Invalid Sector"), m_szDescription("Invalid sector."), m_szScriptPath("data/scripts/systems/null_sector.lua"), m_iGoverningFaction(0)
{
	
}

CSector::CSector(std::string const &szName,
	std::string const &szDesc,
	std::string const &szScriptPath,
	FactionId const iGoverningFaction,
	float const (&flBgTint)[3])
	: m_szName(szName),
	m_szDescription(szDesc),
	m_szScriptPath(szScriptPath),
	m_iGoverningFaction(iGoverningFaction)
{
	this->m_flBgTint[0] = flBgTint[0];
	this->m_flBgTint[1] = flBgTint[1];
	this->m_flBgTint[2] = flBgTint[2];
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

void CSector::get_bg_tint(float (&flBgTint)[3]) const
{
	flBgTint[0] = this->m_flBgTint[0];
	flBgTint[1] = this->m_flBgTint[1];
	flBgTint[2] = this->m_flBgTint[2];
}