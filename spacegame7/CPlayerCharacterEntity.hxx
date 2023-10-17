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