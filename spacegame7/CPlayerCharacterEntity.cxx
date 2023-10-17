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