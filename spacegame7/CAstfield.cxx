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
#include "CAstfield.hxx"

CAstfield::CAstfield()
{

}

CAstfield::CAstfield(CAstfield::CreationParameters const &creationParms)
	: m_szName(creationParms.szName),
	m_szDescription(creationParms.szDescription),
	m_matType(creationParms.matType),
	m_flDropProbability(creationParms.flDropProbability),
	m_uiMinDropQuantity(creationParms.uiMinDropQuantity),
	m_uiMaxDropQuantity(creationParms.uiMaxDropQuantity),
	m_flPirateAttackChancePerTick(creationParms.flPirateAttackChancePerTick),
	m_vPirateEncounters(creationParms.vPirateEncounters),
	m_flRadius(creationParms.flRadius)
{

}

CAstfield::CAstfield(
	std::string const& szName,
	std::string const& szDesc,
	MaterialType const matType,
	float const flDropProbability,
	unsigned int const uiMinDropQuantity,
	unsigned int const uiMaxDropQuantity,
	float const flPirateAttackChancePerTick,
	std::vector<CharacterId> const& vPirateEncounters,
	float const flRadius
)
	: m_szName(szName),
	m_szDescription(szDesc),
	m_matType(matType),
	m_flDropProbability(flDropProbability),
	m_uiMinDropQuantity(uiMinDropQuantity),
	m_uiMaxDropQuantity(uiMaxDropQuantity),
	m_flPirateAttackChancePerTick(flPirateAttackChancePerTick),
	m_vPirateEncounters(vPirateEncounters),
	m_flRadius(flRadius)
{

}

CAstfield::~CAstfield()
{

}

std::string CAstfield::get_name(void) const
{
	return this->m_szName;
}


std::string CAstfield::get_desc(void) const
{
	return this->m_szDescription;
}

MaterialType CAstfield::get_mat_type(void) const
{
	return this->m_matType;
}

float CAstfield::get_drop_probability(void) const
{
	return this->m_flDropProbability;
}

unsigned int CAstfield::get_min_drop_quantity(void) const
{
	return this->m_uiMinDropQuantity;
}

unsigned int CAstfield::get_max_drop_quantity(void) const
{
	return this->m_uiMaxDropQuantity;
}

float CAstfield::get_pirate_attack_chance_per_tick(void) const
{
	return this->m_flPirateAttackChancePerTick;
}

float CAstfield::get_radius(void) const
{
	return this->m_flRadius;
}

std::vector<CharacterId> CAstfield::get_pirate_encounters(void) const
{
	return this->m_vPirateEncounters;
}