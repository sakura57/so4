#pragma once

#include "CBaseCharacterEntity.hxx"
#include "IMaterialBank.hxx"

class CPlayerCharacterEntity : public CBaseCharacterEntity
{
public:
	CPlayerCharacterEntity(Loadout const *, std::string const &, bool const);
	virtual ~CPlayerCharacterEntity();

	virtual IEntityInventory *get_inventory(void);
	virtual IMaterialBank* get_material_bank(void);

protected:
	IEntityInventory *m_pInventory;
	IMaterialBank* m_pMaterialBank;
};