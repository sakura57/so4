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
#include "Faction.hxx"
#include "CSector.hxx"
#include "CBase.hxx"
#include "CAstfield.hxx"

#define MAXIMUM_SECTORS 1024

typedef int SectorId;
typedef int BaseId;

interface IUniverse
{
	virtual void add_sector(SectorId const, std::string const &, std::string const &, std::string const &, FactionId const, float const (&)[3]) = 0;
	virtual void add_base(BaseId const, CBase::CreationParameters const &) = 0;
	virtual void add_astfield(AstfieldId const, CAstfield::CreationParameters const&) = 0;
	virtual CSector const &get_sector(SectorId const) const = 0;
	virtual CBase const &get_base(BaseId const) const = 0;
	virtual CAstfield const& get_astfield(AstfieldId const) const = 0;
};