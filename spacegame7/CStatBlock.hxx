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

#include <unordered_map>

enum class Stat : unsigned int
{
	INVALID,
	EVASION,
	HULL_INTEGRITY,
	SHIELD_KNOWLEDGE,
	LASER_PROFICIENCY,
	PLASMA_PROFICIENCY,
	KINETIC_PROFICIENCY,
	MISSILE_PROFICIENCY,
	NEGOTIATION,
	HACKING,
	LUCK
};

class CStatBlock
{
public:
	CStatBlock();
	~CStatBlock();

	void set_stat(Stat const, float const);
	float get_stat(Stat const) const;
	void format_stat_text(std::string &);
	void format_stat_data(std::string&);

	void merge(CStatBlock const *);
	bool empty(void);

	static Stat get_stat_from_name(std::string const &);
	static std::string get_stat_long_name(Stat const);
	static std::string get_stat_short_name(Stat const);
private:
	std::unordered_map<Stat, float> m_mStatMap;
};