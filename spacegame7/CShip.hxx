#pragma once

#include "Defs.hxx"
#include "CEquippedObject.hxx"

#include "Archetype.hxx"
#include "IRadarVisible.hxx"
#include "Listeners.hxx"

//increase to make projectiles have less effect
//on this ship's attitude towards other ships
#define ATTITUDE_DAMAGE_TOLERANCE 500.0f

class CShip : public CEquippedObject, IRadarVisible, public CListenable
{
public:
	enum
	{
		InstanceFlag = 1 << 7
	};

	struct CreationParameters : public CEquippedObject::CreationParameters
	{
		ShipArch const *pArch;
		float flHitPoints;
		float flShieldPoints;
		std::string szName;
		std::string szRadarClass;
		bool bIsPlayer;
		unsigned int uiInvulnerable;
		DropTableId uiDropTable;
	};

	CShip(void);
	virtual ~CShip(void);

	virtual void instance_create(InstanceId const) final;

	virtual void instance_destroy(InstanceId const) final;

	//members inherited from IPhysicalObject
	virtual void physics_tick(float const);

	//members inherited from ILiving
	virtual void alive_tick(float const) final;

	//members defined in CShip
	virtual void initialize(CreationParameters const &);
	virtual ShipArch const *get_ship_arch(void) const;

	std::string get_name(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_szName;
	};

	virtual float get_hit_pts(void);
	virtual void set_hit_pts(float const);
	virtual float get_shield_pts(void);
	virtual void set_shield_pts(float const);
	virtual float get_relative_health(void);
	virtual void set_relative_health(float const);

	float get_throttle(void);
	void set_throttle(float const);
	void set_throttle_unclamped(float const);
	float get_strafe_throttle(void);
	void set_strafe_throttle(float const);
	float get_spin_throttle(void);
	void set_spin_throttle(float const);

	virtual void collision_callback(IWorldObject *);

	virtual unsigned int get_physics_type(void) const
	{
		return PhysicsType::Collidable | PhysicsType::HasCallback | PhysicsType::Solid;
	};

	virtual std::string get_object_name(void)
	{
		return "Ship";
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

	virtual IRadarVisible *get_radar(void)
	{
		return this;
	};

	virtual std::string get_radar_class(void)
	{
		return this->m_szRadarClass;
	};

	bool shields_up(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_flShieldPoints > 0.0f;
	};

	InstanceId get_target(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		if(this->m_iTargetId != INVALID_INSTANCE && SG::get_engine()->instance_is_allocated(this->m_iTargetId))
		{
			return this->m_iTargetId;
		}

		this->m_iTargetId = INVALID_INSTANCE;

		return INVALID_INSTANCE;
	};

	void set_target(InstanceId const iInstanceId)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		this->m_iTargetId = iInstanceId;
	};

	void initiate_jump(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		this->m_bIsJumping = true;
	};

	bool is_jumping(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_bIsJumping;
	};

	bool is_player(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_bIsPlayer;
	};

	void flick_shields(void);

	void inflict_damage(float const flHullDamage, float const flShieldDamage);

	void set_invulnerable(unsigned int const);

protected:
	//should not be changed after initialize()
	ShipArch const *m_pShipArch;

	unsigned int m_uiInvulnerable;

	DropTableId m_uiDropTable;

	bool m_bIsJumping;
	bool m_bIsPlayer;
	bool m_bSmokeEmitted;
	float m_flSmokeTimer;

	float m_flHitPoints;
	float m_flShieldPoints;
	float m_flThrottle;
	float m_flSpinThrottle;
	float m_flStrafeThrottle;

	float m_flShieldRegenTimeRemaining;

	InstanceId m_iTargetId;

	std::string m_szName;
	std::string m_szRadarClass;

	InstanceId m_iEngineFlameInstanceId;
	InstanceId m_iShieldInstanceId;

	SoundInstanceId m_iThrusterSound;

private:
	IWorldObject *get_shield(void)
	{
		IWorldInstance *pInstance = SG::get_engine()->instance_get(this->m_iShieldInstanceId);

		if (pInstance->instance_get_flags() & IWorldObject::InstanceFlag)
		{
			return static_cast<IWorldObject*>(pInstance);
		}
		else
		{
			return nullptr;
		}
	};

	void take_damage(float const flHullDamage, float const flShieldDamage);

	void ship_destroy(void);
};
