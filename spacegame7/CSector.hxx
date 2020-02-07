#pragma once

#include "Defs.hxx"
#include <string>
#include "Faction.hxx"

class CSector
{
public:
	CSector();
	CSector(std::string const &, std::string const &, std::string const &, FactionId const, float const (&)[3]);
	~CSector();

	std::string get_name(void) const;
	std::string get_desc(void) const;
	std::string get_script_path(void) const;
	FactionId get_governing_faction(void) const;
	void get_bg_tint(float (&)[3]) const;

private:
	std::string m_szName;
	std::string m_szDescription;
	std::string m_szScriptPath;
	FactionId m_iGoverningFaction;

	float m_flBgTint[3];
};