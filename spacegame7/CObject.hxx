#pragma once

#include <mutex>

#include "Defs.hxx"
#include "Util.hxx"
#include "EngineProtos.hxx"
#include "IWorldObject.hxx"

/*
 * Implementation of the base world object,
 * possessing location and physical properties.
 * 
 * Thread-safe, because an object may be accessed
 * from multiple threads (rendering thread, physics thread)
 * 
 * NOTE: Since Verlet does not explicitly track velocities,
 * set_velocity and set_angular_velocity in CObject and all
 * derivatives have no effect. However, getters should still
 * work properly.
 */
class CObject : public IWorldObject, public IPhysicalObject
{
public:
	enum
	{
		InstanceFlag = 1 << 3
	};
    
    struct CreationParameters
    {
		float flRadius;
        AppearanceId uiAppearanceId;
        Vector2f vPosition;
		float flRotation;
		Vector2f vVelocity;
		float flAngularVelocity;

		float flMass;
		float flRotationalInertia;
    };

	CObject(void);

	//members inherited from IWorldInstance
	virtual void instance_create(InstanceId const);
	virtual void instance_destroy(InstanceId const);
	virtual InstanceId instance_get_id(void) final;
	virtual InstanceFlags instance_get_flags(void) final;
    virtual void mark_for_deletion(void) final;
    virtual bool is_marked_for_deletion(void) final;
	virtual void instance_acquired(void) final;
	virtual void instance_released(void) final;
	virtual bool is_safely_destroyable(void) final;
    
    //members inherited from IWorldObject
	virtual ~CObject(void);
	virtual Vector2f get_position(void);
	virtual void set_position(Vector2f const &);
	virtual float get_rotation(void);
	virtual void set_rotation(float const);
	virtual AppearanceId get_appearance(void);
	virtual void set_appearance(AppearanceId) final;
	virtual bool is_initialized(void) final;

	//members inherited from IPhysicalObject
	virtual void physics_tick(float const);
	virtual float get_radius(void);
	virtual void set_radius(float const);
	virtual Vector2f get_velocity(void);
	virtual void set_velocity(Vector2f const &);
	virtual Vector2f get_acceleration(void);
	virtual void set_acceleration(Vector2f const &);
	virtual float get_angular_velocity(void);
	virtual void set_angular_velocity(float const);
	virtual float get_angular_acceleration(void);
	virtual void set_angular_acceleration(float const);

	virtual float get_mass(void);
	virtual void set_mass(float const);
	virtual float get_rotational_inertia(void);
	virtual void set_rotational_inertia(float const);
    
    //members defined in CObject
    virtual void initialize(CreationParameters const &);

	virtual unsigned int get_physics_type(void) const
	{
		return PhysicsType::NonCollidable;
	};

	virtual void collision_callback(IWorldObject *);

	virtual IPhysicalObject *get_physical(void)
	{
		return this;
	};

	virtual std::string get_object_name(void)
	{
		return "Object";
	};

protected:
	//since CObject's functions are not time-consuming,
	//mutual exclusion is provided by a spinlock.
	Spinlock m_mFieldAccess;

	bool m_bInitialized;

	std::atomic<unsigned int> m_uiReferenceCount;

    bool m_bMarkedForDeletion;
	InstanceId m_iInstanceId;
	InstanceFlags m_uiInstanceFlags;
	
	float m_flRadius;
	AppearanceId m_uiAppearanceId;
	Vector2f m_vPosition;
	float m_flRotation;
	Vector2f m_vVelocity;
	float m_flAngularVelocity;
	Vector2f m_vAcceleration;
	float m_flAngularAcceleration;

	float m_flMass;
	float m_flRotationalInertia;

	Vector2f estimate_velocity(void)
	{
		return this->m_vVelocity;
	};

private:
	//needed for TCV
	//float m_flOldDelta;
	//Vector2f m_vOldPosition;
	//float m_flOldRotation;

	float m_flOldAngularAccel;
	float m_flOldAngularVelocity;
	float m_flOldRotation;

	Vector2f m_vOldAccel;
	Vector2f m_vOldVelocity;
	Vector2f m_vOldPosition;
};
