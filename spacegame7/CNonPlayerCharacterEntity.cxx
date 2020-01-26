#include "CNonPlayerCharacterEntity.hxx"
#include "CEntityInventory.hxx"

CNonPlayerCharacterEntity::CNonPlayerCharacterEntity(Loadout const *pLoadout, std::string const &szName, bool const bPlayer)
	: CBaseCharacterEntity(pLoadout, szName, bPlayer)
{
}

CNonPlayerCharacterEntity::~CNonPlayerCharacterEntity()
{
}