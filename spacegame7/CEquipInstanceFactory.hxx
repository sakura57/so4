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

#include "IEngine.hxx"
#include "SGEngine.hxx"
#include "SGLib.hxx"
#include "CInstanceFactory.hxx"
#include "CEquipInstance.hxx"
#include "CShip.hxx"

/*
 * CEquipInstanceFactory
 * 
 * Factory of EquipInstance-based objects.
 */
class CEquipInstanceFactory
{
public:
	//factory only provides methods; no instantiation
	CEquipInstanceFactory() = delete;
	CEquipInstanceFactory(CEquipInstanceFactory const &) = delete;
	~CEquipInstanceFactory() = delete;

	INLINE static InstanceId create(CEquippedObject *pParent, ArchId const archId)
	{
		Archetype const * pArch = SG::get_game_data_manager()->get_arch<Archetype>(archId);
		InstanceId instanceId;

		switch(pArch->uiArchType)
		{
			case ARCH_WEAPON:
			case ARCH_MISSILE:
			{
				CEquipInstanceFactory::create_delegate<CWeaponInstance>(pParent, instanceId, pArch);
				break;
			}
			default:
			{
				break;
			}
		}

		return instanceId;
	};

	/*
	* create an instance of class T. Takes care of adding the
	* object to the world.
	* 
	* the template parameter may NOT be a base class
	* or interface.
	*
	* This function invokes instance_request(),
	* which may throw an SGException if allocation fails,
	* for instance, due to the pool being full.
	*
	* A compile-time error will be generated if a
	* type has been supplied to the template that
	* cannot be allocated.
	*/
private:
	template<class T>
	INLINE static void create_delegate(CEquippedObject *pParent, InstanceId &iInstance, Archetype const *pArch)
	{
		T * pInstance;

		CInstanceFactory::create<T>(iInstance, pInstance);
		T::CreationParameters parms;
		parms.pArch = pArch;
		parms.pParent = pParent;
		pInstance->initialize(parms);
		pInstance->alive_set(true);

		if(pParent->instance_get_flags() & CShip::InstanceFlag)
		{
			std::vector<Vector2f> const &vOffsets = reinterpret_cast<CShip*>(pParent)->get_ship_arch()->hardpoints;

			pInstance->set_child_spawn_offset(vOffsets[pParent->get_children_count()]);
		}

		SG::get_world()->instance_enqueue(pInstance);
	};
};