#pragma once

#include "Defs.hxx"
#include "CEquippedObject.hxx"

#include "Archetype.hxx"
#include "IRadarVisible.hxx"
#include "IUniverse.hxx"

typedef unsigned int AstfieldId;

class CAsteroidMineable : public CEquippedObject, IRadarVisible
{
public:
	enum
	{
		InstanceFlag = 1 << 21
	};

	struct CreationParameters : public CEquippedObject::CreationParameters
	{
		std::string szName;
		std::string szRadarClass;
		std::string szMaterialSerial;
		unsigned int uiMinQuantity;
		unsigned int uiMaxQuantity;
		float flDropProbability;
		AstfieldId uiAstfieldId;
	};

	CAsteroidMineable(void);
	virtual ~CAsteroidMineable(void);

	virtual void instance_create(InstanceId const) final;

	virtual void instance_destroy(InstanceId const) final;

	//members inherited from IPhysicalObject
	virtual void physics_tick(float const);

	//members inherited from ILiving
	virtual void alive_tick(float const) final;

	//members defined in CAsteroidMineable
	virtual void initialize(CreationParameters const&);

	std::string get_name(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_szName;
	};

	virtual unsigned int get_physics_type(void) const
	{
		return PhysicsType::Collidable | PhysicsType::HasCallback;
	};

	virtual std::string get_object_name(void)
	{
		return "Asteroid";
	};

	virtual RadarShape get_radar_shape(void)
	{
		return RadarShape::Circle;
	};

	virtual std::string get_radar_name(void)
	{
		if(this->m_pFaction == nullptr)
		{
			return this->m_szRadarClass;
		}

		return this->m_pFaction->short_name + " - " + this->m_szRadarClass;
	};

	virtual IRadarVisible* get_radar(void)
	{
		return this;
	};

	virtual std::string get_radar_class(void)
	{
		return this->m_szRadarClass;
	};

	/*
	* The interact() function from IInteractable.
	*
	* When this is called by the game thread due to user input,
	* bring up the dialog confirming jump sequence.
	*/
	virtual void interact(InstanceId const);

	virtual void collision_callback(IWorldObject*);

protected:
	//should not be changed after initialize()
	std::string m_szName;
	std::string m_szRadarClass;

private:
	void mine_asteroid(void);

	bool m_bUsed;
	InstanceId m_iInteractorId;
	std::string m_szMaterialSerial;
	AstfieldId m_uiAstfieldId;
	unsigned int m_uiMinQuantity;
	unsigned int m_uiMaxQuantity;
	float m_flDropProbability;
	float m_flBreakProbability;
};