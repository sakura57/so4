#pragma once

#include "CBaseCharacterEntity.hxx"

class CNonPlayerCharacterEntity : public CBaseCharacterEntity
{
public:
	CNonPlayerCharacterEntity(Loadout const *, std::string const &, bool const);
	virtual ~CNonPlayerCharacterEntity();
protected:
};