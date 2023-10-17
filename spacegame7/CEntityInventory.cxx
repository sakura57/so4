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
#include <mutex>
#include "CEntityInventory.hxx"

void CEntityInventory::add_item(ArchId const iItemId, unsigned int const uiQuantity)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_quantityMap[iItemId] += uiQuantity;
}

void CEntityInventory::add_items(QuantityMap const &quantityMap)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	QuantityMap::const_iterator i;

	for(i = quantityMap.begin(); i != quantityMap.end(); ++i)
	{
		this->m_quantityMap[i->first] += i->second;
	}
}

void CEntityInventory::remove_item(ArchId const iItemId, unsigned int const uiQuantity)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	auto i = this->m_quantityMap.find(iItemId);

	if (i == this->m_quantityMap.end())
	{
		throw SGException("attempt to remove item from inventory which was not in the inventory");
	}

	if(i->second < uiQuantity)
	{
		throw SGException("attempt to remove item resulted in negative inventory quantity");
	}
	else
	{
		i->second -= uiQuantity;
	}

	if(i->second == 0)
	{
		this->m_quantityMap.erase(i);
	}
}

int CEntityInventory::get_money(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_iMoney;
}

void CEntityInventory::set_money(int const iMoney)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_iMoney = iMoney;
}

void CEntityInventory::adjust_money(int const iMoney)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_iMoney += iMoney;
}

int CEntityInventory::get_metal(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_iMetal;
}

void CEntityInventory::set_metal(int const iMetal)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_iMetal = iMetal;
}

void CEntityInventory::adjust_metal(int const iMetal)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_iMetal += iMetal;
}

unsigned int CEntityInventory::get_item_quantity(ArchId const archId)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	auto i = this->m_quantityMap.find(archId);

	if(i == this->m_quantityMap.end())
	{
		return 0;
	}

	return i->second;
}

void CEntityInventory::get_items_in_inventory(ItemList &itemList)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	for(auto i : this->m_quantityMap)
	{
		itemList.push_back(i);
	}
}

void CEntityInventory::clear_items(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_quantityMap.clear();
}