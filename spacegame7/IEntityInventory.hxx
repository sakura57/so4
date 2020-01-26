#pragma once

#include "Util.hxx"
#include "EngineProtos.hxx"
#include <vector>
#include <unordered_map>

#define MAX_INVENTORY_ITEMS 100

typedef std::vector<std::pair<ArchId, unsigned int>> ItemList;
typedef std::unordered_map<ArchId, unsigned int> QuantityMap;

interface IEntityInventory
{
	/*
	 * Add a given quantity of an item to the inventory.
	 */
	virtual void add_item(ArchId const, unsigned int const) = 0;

	/*
	 * Remove a given quantity of an item from the inventory.
	 */
	virtual void remove_item(ArchId const, unsigned int const) = 0;

	/*
	 * Adds the given items to the inventory;
	 */
	virtual void add_items(QuantityMap const &) = 0;

	/*
	 * Get the money in this inventory
	 */
	virtual int get_money(void) = 0;

	/*
	 * Set the money in this inventory
	 */
	virtual void set_money(int const) = 0;

	/*
	 * Adjust money in this inventory, either by a positive or negative amount
	 */
	virtual void adjust_money(int const) = 0;

	/*
	* Get the metal in this inventory
	*/
	virtual int get_metal(void) = 0;

	/*
	* Set the metal in this inventory
	*/
	virtual void set_metal(int const) = 0;

	/*
	* Adjust metal in this inventory, either by a positive or negative amount
	*/
	virtual void adjust_metal(int const) = 0;

	/*
	 * Return the quantity of a given item in the inventory.
	 * (slow, do not call each tick)
	 */
	virtual unsigned int get_item_quantity(ArchId const) = 0;

	/*
	 * Return a vector containing all items in the inventory.
	 */
	virtual void get_items_in_inventory(ItemList &) = 0;

	virtual void clear_items(void) = 0;
};