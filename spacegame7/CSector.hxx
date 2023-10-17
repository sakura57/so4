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