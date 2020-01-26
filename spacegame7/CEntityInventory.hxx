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