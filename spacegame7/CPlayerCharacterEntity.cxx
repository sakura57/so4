#include "CPlayerCharacterEntity.hxx"
#include "CEntityInventory.hxx"
#include "CMaterialBank.hxx"

CPlayerCharacterEntity::CPlayerCharacterEntity(Loadout const *pLoadout, std::string const &szName, bool const bPlayer)
	: CBaseCharacterEntity(pLoadout, szName, bPlayer)
{
	this->m_pInventory = new CEntityInventory;
	this->m_pMaterialBank = new CMaterialBank;
}

CPlayerCharacterEntity::~CPlayerCharacterEntity()
{
	delete this->m_pInventory;
	this->m_pInventory = nullptr;

	delete this->m_pMaterialBank;
	this->m_pMaterialBank = nullptr;
}

IEntityInventory *CPlayerCharacterEntity::get_inventory(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_pInventory;
}

IMaterialBank* CPlayerCharacterEntity::get_material_bank(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_pMaterialBank;
}