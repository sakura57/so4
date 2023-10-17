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
#include "CStatBlock.hxx"
#include <sstream>

CStatBlock::CStatBlock()
{
	
}

CStatBlock::~CStatBlock()
{
	
}

float CStatBlock::get_stat(Stat const stat) const
{
	auto i = this->m_mStatMap.find(stat);

	if(i == this->m_mStatMap.end())
	{
		return 0.0f;
	}

	return i->second;
}

void CStatBlock::set_stat(Stat const stat, float const flValue)
{
	this->m_mStatMap[stat] = flValue;
}

std::string CStatBlock::get_stat_short_name(Stat const stat)
{
	static std::unordered_map<Stat, std::string> const statNameMap(
		{
			{ Stat::EVASION, "EVASION" },
			{ Stat::HULL_INTEGRITY, "HULL_INTEGRITY" },
			{ Stat::SHIELD_KNOWLEDGE, "SHIELD_KNOWLEDGE" },
			{ Stat::LASER_PROFICIENCY, "LASER_PROFICIENCY" },
			{ Stat::PLASMA_PROFICIENCY, "PLASMA_PROFICIENCY" },
			{ Stat::KINETIC_PROFICIENCY, "KINETIC_PROFICIENCY" },
			{ Stat::MISSILE_PROFICIENCY, "MISSILE_PROFICIENCY" },
			{ Stat::NEGOTIATION, "NEGOTIATION" },
			{ Stat::HACKING, "HACKING" },
			{ Stat::LUCK, "LUCK"}
		});

	auto i = statNameMap.find(stat);

	if (i == statNameMap.end())
	{
		return std::string("INVALID_STAT");
	}

	return i->second;
}

Stat CStatBlock::get_stat_from_name(std::string const &szStatName)
{
	static std::unordered_map<std::string, Stat> const statNameMap(
	{
		{ "EVASION", Stat::EVASION },
		{ "HULL_INTEGRITY", Stat::HULL_INTEGRITY },
		{ "SHIELD_KNOWLEDGE", Stat::SHIELD_KNOWLEDGE },
		{ "LASER_PROFICIENCY", Stat::LASER_PROFICIENCY },
		{ "PLASMA_PROFICIENCY", Stat::PLASMA_PROFICIENCY },
		{ "KINETIC_PROFICIENCY", Stat::KINETIC_PROFICIENCY },
		{ "MISSILE_PROFICIENCY", Stat::MISSILE_PROFICIENCY },
		{ "NEGOTIATION", Stat::NEGOTIATION },
		{ "HACKING", Stat::HACKING },
		{ "LUCK", Stat::LUCK}
	});

	auto i = statNameMap.find(szStatName);

	if(i == statNameMap.end())
	{
		return Stat::INVALID;
	}

	return i->second;
}

std::string CStatBlock::get_stat_long_name(Stat const stat)
{
	static std::unordered_map<Stat, std::string> const statNameMap(
	{
		{ Stat::EVASION, "Evasion" },
		{ Stat::HULL_INTEGRITY, "Hull Integrity" },
		{ Stat::SHIELD_KNOWLEDGE, "Shield Knowledge" },
		{ Stat::LASER_PROFICIENCY, "Laser Proficiency" },
		{ Stat::PLASMA_PROFICIENCY, "Plasma Proficiency" },
		{ Stat::KINETIC_PROFICIENCY, "Kinetic Proficiency" },
		{ Stat::MISSILE_PROFICIENCY, "Missile Proficiency" },
		{ Stat::NEGOTIATION, "Bartering" },
		{ Stat::HACKING, "Hacking" },
		{ Stat::LUCK, "Luck" }
	});

	auto i = statNameMap.find(stat);

	if(i == statNameMap.end())
	{
		return "Invalid Stat";
	}

	return i->second;
}

void CStatBlock::merge(CStatBlock const *pOther)
{
	for (auto stat : pOther->m_mStatMap)
	{
		this->m_mStatMap[stat.first] += stat.second;
	}
}

void CStatBlock::format_stat_data(std::string& s)
{
	std::stringstream ss;

	bool bFirst = true;

	for (auto stat : this->m_mStatMap)
	{
		if(!bFirst) ss << " ";

		ss << CStatBlock::get_stat_short_name(stat.first);
		ss << " ";
		ss << stat.second;

		if (bFirst)
		{
			bFirst = false;
		}
	}

	s = ss.str();
}

void CStatBlock::format_stat_text(std::string &s)
{
	std::stringstream ss;

	for (auto stat : this->m_mStatMap)
	{
		ss << " ";
		ss << CStatBlock::get_stat_long_name(stat.first);
		ss << " +";
		ss << stat.second;
		ss << "\n";
	}
	
	s = ss.str();
}

bool CStatBlock::empty(void)
{
	return this->m_mStatMap.size() == 0;
}