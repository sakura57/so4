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

#include "IUniverse.hxx"
#include "CSector.hxx"

class CUniverse : public IUniverse
{
public:
	CUniverse();
	~CUniverse();

	virtual void add_sector(SectorId const, std::string const &, std::string const &, std::string const &, FactionId const, float const (&)[3]) final;
	virtual void add_base(BaseId const, CBase::CreationParameters const &) final;
	virtual void add_astfield(AstfieldId const, CAstfield::CreationParameters const&) final;
	virtual CSector const &get_sector(SectorId const) const final;
	virtual CBase const &get_base(BaseId const) const final;
	virtual CAstfield const& get_astfield(AstfieldId const) const final;
private:
	std::vector<CSector> m_vSectors;
	std::vector<CBase> m_vBases;
	std::vector<CAstfield> m_vAstfields;
};