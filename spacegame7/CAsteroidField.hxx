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

#include "ILiving.hxx"
#include "CPhantomObject.hxx"
#include "MaterialType.hxx"
#include <vector>

class CAsteroidField : public CPhantomObject, public virtual ILiving
{
public:
	enum
	{
		InstanceFlag = 1 << 22
	};

	struct CreationParameters : CPhantomObject::CreationParameters
	{
		AstfieldId uiAstfieldInstanceId;
		float flDropProbability;
		MaterialType matType;
		unsigned int uiDropMinQuantity;
		unsigned int uiDropMaxQuantity;
		float flPirateAttackChancePerTick;
		std::vector<CharacterId> vPirateEncounterCharacters;
		float flRadius;
		Vector2f vPosition;
	};

	CAsteroidField(void);

	//members inherited from IWorldInstance
	virtual void instance_create(InstanceId const) final;
	virtual void instance_destroy(InstanceId const) final;

	//members inherited from IWorldObject
	virtual ~CAsteroidField(void);
	virtual Vector2f get_position(void) final;
	virtual void set_position(Vector2f const&) final;
	virtual float get_rotation(void) final;
	virtual void set_rotation(float const) final;
	virtual AppearanceId get_appearance(void) final;
	virtual void set_appearance(AppearanceId) final;

	//members inherited from ILiving
	virtual void alive_tick(float const) final;
	virtual void alive_set(bool const) final;

	virtual bool is_alive(void) final
	{
		return true;
	};

	//members defined in CAsteroidField
	virtual void initialize(CreationParameters const&);

	size_t get_children_count(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_lChildIds.size();
	};

	std::vector<InstanceId> get_children(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_lChildIds;
	};

protected:
	/*
	 * Child instances should call this when destroyed, so child objects
	 * can be removed.
	 */
	void notify_deletion()
	{
		//Walk child instances
		for(InstanceId childId : this->m_lChildIds)
		{
			//Retrieve the world instance
			IWorldInstance* pChildInstance = SG::get_engine()->instance_get(childId);

			if(pChildInstance != nullptr)
			{
				pChildInstance->mark_for_deletion();
			}
		}
	}

private:
	void spawn_asteroid_statics(void);
	void update_dynamic_asteroids(float const);
	InstanceId create_randomized_dynast(void);

	std::vector<InstanceId> m_lChildIds;

	float m_flDynastsTimer;

	float m_flDropProbability;
	MaterialType m_matType;
	AstfieldId m_uiAstfieldInstanceId;
	unsigned int m_uiDropMinQuantity;
	unsigned int m_uiDropMaxQuantity;
	unsigned int m_uiMaxDynamicAsteroids;
	float m_flPirateAttackChancePerTick;
	std::vector<CharacterId> m_vPirateEncounterCharacters;
	float m_flRadius;
	Vector2f m_vPosition;
	std::string m_szMaterialSerial;

	bool m_bPlayerPresent;
};