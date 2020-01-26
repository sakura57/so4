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