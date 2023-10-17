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

#include "IEntityInventory.hxx"

/*
 * Class representing a null inventory.
 * The null inventory is simply a void, items and money
 * deposited are not retained.
 */
class CNullInventory : public IEntityInventory
{
public:
	virtual void add_item(ArchId const, unsigned int const) {};
	virtual void remove_item(ArchId const, unsigned int const) {};

	virtual int get_money(void)
	{
		return 0;
	};

	virtual void set_money(int const) {};
	virtual void adjust_money(int const) {};

	virtual unsigned int get_item_quantity(ArchId const)
	{
		return 0;
	};

	virtual void get_items_in_inventory(ItemList &) {};

private:
};