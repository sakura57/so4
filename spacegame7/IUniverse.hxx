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