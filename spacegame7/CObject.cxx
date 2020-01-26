#include "CInstanceFactory.hxx"
#include "CObject.hxx"
#include <iostream>

CObject::CObject(void)
{
	this->m_bInitialized = false;
}

CObject::~CObject(void)
{
	//we have nothing to free
}

void CObject::initialize(CObject::CreationParameters const &parms)
{
	this->m_flRadius = parms.flRadius;
	this->m_uiAppearanceId = parms.uiAppearanceId;
	this->m_vPosition = parms.vPosition;
	this->m_flRotation = parms.flRotation;
	this->m_vVelocity = parms.vVelocity;
	this->m_flAngularVelocity = parms.flAngularVelocity;
	this->m_vAcceleration = Vector2f(0.0f, 0.0f);
	this->m_flAngularAcceleration = 0.0f;

	this->m_flMass = parms.flMass;
	this->m_flRotationalInertia = parms.flRotationalInertia;

	//this->m_flOldDelta = GAME_INITIAL_DELTA;
	this->m_vOldPosition = parms.vPosition - (GAME_INITIAL_DELTA * parms.vVelocity);
	this->m_flOldRotation = parms.flRotation;
	this->m_vOldVelocity = parms.vVelocity;
	this->m_flOldAngularVelocity = 0.0f;
	this->m_vOldAccel = Vector2f(0.0f, 0.0f);
	this->m_flOldAngularAccel = 0.0f;

	this->m_bInitialized = true;
}


void CObject::instance_create(InstanceId const instanceId)
{
	this->m_iInstanceId = instanceId;

	this->m_uiInstanceFlags =
        IWorldInstance::InstanceFlag |
        IWorldObject::InstanceFlag |
        IPhysicalObject::InstanceFlag |
        CObject::InstanceFlag;
        
	this->m_uiReferenceCount = 0;
    this->m_bMarkedForDeletion = false;
}

void CObject::instance_destroy(InstanceId const instanceId)
{
	if(this->m_iInstanceId == instanceId)
	{
		this->m_iInstanceId = -1;
	}
}

InstanceFlags CObject::instance_get_flags(void)
{
	return this->m_uiInstanceFlags;
}


InstanceId CObject::instance_get_id(void)
{
	return this->m_iInstanceId;
}

void CObject::mark_for_deletion(void)
{
    SCOPE_LOCK(this->m_mFieldAccess);
    
    this->m_bMarkedForDeletion = true;
}

bool CObject::is_marked_for_deletion(void)
{
    SCOPE_LOCK(this->m_mFieldAccess);
    
    return this->m_bMarkedForDeletion;
}


void CObject::instance_acquired(void)
{
	//SCOPE_LOCK(this->m_mFieldAccess);

	++this->m_uiReferenceCount;
}

void CObject::instance_released(void)
{
	//SCOPE_LOCK(this->m_mFieldAccess);

	if(this->m_uiReferenceCount) --this->m_uiReferenceCount;
}

bool CObject::is_safely_destroyable(void)
{
	//SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_uiReferenceCount == 0;
}

Vector2f CObject::get_position(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_vPosition;
}

void CObject::set_position(Vector2f const &newPosition)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vPosition = newPosition;
}

float CObject::get_rotation(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_flRotation;
}

void CObject::set_rotation(float const newRotation)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flRotation = newRotation;
}

Vector2f CObject::get_velocity(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	//return (this->m_vPosition - this->m_vOldPosition) / (this->m_flOldDelta);

	return this->m_vVelocity;
}

/*
 * Note: this method has no effect.
 */
void CObject::set_velocity(Vector2f const &newVelocity)
{
	this->m_vVelocity = newVelocity;
	this->m_vOldVelocity = newVelocity;
}

/*
 * Note: this method returns an APPROXIMATION.
 */
float CObject::get_angular_velocity(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	//return (this->m_flRotation - this->m_flOldRotation) / (this->m_flOldDelta);

	return this->m_flAngularVelocity;
}

/*
* Note: this method has no effect.
*/
void CObject::set_angular_velocity(float const newAngularVelocity)
{
	//NO EFFECT
}

Vector2f CObject::get_acceleration(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_vAcceleration;
}

void CObject::set_acceleration(Vector2f const &newAcceleration)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vAcceleration = newAcceleration;
}

float CObject::get_angular_acceleration(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_flAngularAcceleration;
}

void CObject::set_angular_acceleration(float const newAngularAcceleration)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flAngularAcceleration = newAngularAcceleration;
}

AppearanceId CObject::get_appearance()
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_uiAppearanceId;
}

void CObject::set_appearance(AppearanceId const newAppearance)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_uiAppearanceId = newAppearance;
}

float CObject::get_radius()
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_flRadius;
}

void CObject::set_radius(float const flRadius)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flRadius = flRadius;
}

float CObject::get_mass(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);
	
	return this->m_flMass;
}

void CObject::set_mass(float const flMass)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flMass = flMass;
}

float CObject::get_rotational_inertia(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);
	
	return this->m_flRotationalInertia;
}

void CObject::set_rotational_inertia(float const flRotationalInertia)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_flRotationalInertia = flRotationalInertia;
}

/*
 * Implementation of TCV to update physics state.
 */
void CObject::physics_tick(float const flDelta)
{
	SCOPE_LOCK(this->m_mFieldAccess);


	Vector2f vVelHalfStep = this->m_vVelocity + ((1.0f / 2.0f) * this->m_vAcceleration * flDelta);
	Vector2f vNewPosition = this->m_vPosition + vVelHalfStep * flDelta;
	Vector2f vNewVelocity = vVelHalfStep + ((1.0f / 2.0f) * this->m_vAcceleration * flDelta);

	float flAngVelHalfStep = this->m_flAngularVelocity + ((1.0f / 2.0f) * this->m_flAngularAcceleration * flDelta);
	float flNewRotation = this->m_flRotation + flAngVelHalfStep * flDelta;
	float flNewAngVel = flAngVelHalfStep + ((1.0f / 2.0f) * this->m_flAngularAcceleration * flDelta);

	this->m_vOldPosition = this->m_vPosition;
	this->m_vOldVelocity = this->m_vVelocity;
	this->m_flOldRotation = this->m_flRotation;
	this->m_flOldAngularVelocity = this->m_flAngularVelocity;

	this->m_vPosition = vNewPosition;
	this->m_vVelocity = vNewVelocity;
	this->m_flRotation = flNewRotation;
	this->m_flAngularVelocity = flNewAngVel;

	if(std::isinf(this->m_vPosition.x) || std::isnan(this->m_vPosition.x) ||
		std::isinf(this->m_vPosition.y) || std::isnan(this->m_vPosition.y))
	{
		this->m_vPosition = this->m_vOldPosition;
	}

	if (std::isinf(this->m_vVelocity.x) || std::isnan(this->m_vVelocity.x) ||
		std::isinf(this->m_vVelocity.y) || std::isnan(this->m_vVelocity.y))
	{
		this->m_vVelocity = this->m_vOldVelocity;
	}

	if (std::isinf(this->m_flRotation) || std::isnan(this->m_flRotation))
	{
		this->m_flRotation = this->m_flOldRotation;
	}
}

bool CObject::is_initialized(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_bInitialized;
}

void CObject::collision_callback(IWorldObject *)
{
	//dummy function
}
