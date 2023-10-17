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
#include "CUniverse.hxx"

CUniverse::CUniverse()
{
	
}

CUniverse::~CUniverse()
{
	
}

void CUniverse::add_sector(SectorId const iSectorId, std::string const &szName, std::string const &szDesc, std::string const &szScriptPath, FactionId const iGoverningFaction, float const (&flBgTint)[3])
{
	if(this->m_vSectors.size() <= iSectorId)
	{
		this->m_vSectors.resize(iSectorId + 1);
	}

	this->m_vSectors[iSectorId] = CSector(szName, szDesc, szScriptPath, iGoverningFaction, flBgTint);
}

CSector const &CUniverse::get_sector(SectorId const iSectorId) const
{
	return this->m_vSectors[iSectorId];
}

void CUniverse::add_base(BaseId const iBaseId, CBase::CreationParameters const &baseCreationParms)
{
	if(this->m_vBases.size() <= iBaseId)
	{
		this->m_vBases.resize(iBaseId + 1);
	}

	this->m_vBases[iBaseId] = CBase(baseCreationParms);
}

CBase const &CUniverse::get_base(BaseId const iSectorId) const
{
	return this->m_vBases[iSectorId];
}

void CUniverse::add_astfield(AstfieldId const iAstfieldId, CAstfield::CreationParameters const& afCreationParms)
{
	if(this->m_vAstfields.size() <= iAstfieldId)
	{
		this->m_vAstfields.resize(iAstfieldId + 1);
	}

	this->m_vAstfields[iAstfieldId] = CAstfield(afCreationParms);
}

CAstfield const& CUniverse::get_astfield(AstfieldId const iAstfieldId) const
{
	return this->m_vAstfields[iAstfieldId];
}