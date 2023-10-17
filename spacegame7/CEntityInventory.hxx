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

#include <unordered_map>
#include "IEntityInventory.hxx"

/*
 * Standard entity inventory.
 */
class CEntityInventory : public IEntityInventory
{
public:
	virtual void add_item(ArchId const, unsigned int const) final;
	virtual void remove_item(ArchId const, unsigned int const) final;
	virtual unsigned int get_item_quantity(ArchId const) final;
	virtual void get_items_in_inventory(ItemList &) final;
	virtual void add_items(QuantityMap const &) final;

	virtual int get_money(void) final;
	virtual void set_money(int const) final;
	virtual void adjust_money(int const) final;
	virtual int get_metal(void) final;
	virtual void set_metal(int const) final;
	virtual void adjust_metal(int const) final;
	virtual void clear_items(void) final;

private:
	Spinlock m_mFieldAccess;

	std::unordered_map<ArchId, unsigned int> m_quantityMap;
	int m_iMoney;
	int m_iMetal;
};