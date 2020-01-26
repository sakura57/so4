#include "CChaseCamera.hxx"

CChaseCamera::CChaseCamera(void)
{
	this->m_pTarget = nullptr;
}

CChaseCamera::~CChaseCamera(void)
{
	
}

void CChaseCamera::acquire_target(IWorldObject *pObject)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	//increment the object's reference count.
	//while ChaseCamera is targeting the object, the object
	//may not be destroyed - ChaseCamera must release
	//the object first
	pObject->instance_acquired();

	this->m_pTarget = pObject;
}

void CChaseCamera::release_target(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(this->m_pTarget)
	{
		IWorldObject *pTarget = this->m_pTarget;
		this->m_pTarget = nullptr;

		pTarget->instance_released();
	}
}

void CChaseCamera::listen_notify(Notification const notification, IListenable *pListenable)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	IWorldObject *pListenableDependent = nullptr;

	switch (notification.m_type)
	{
	case Notification::Type::DEPENDENT_DESTROYED:
		pListenableDependent = dynamic_cast<IWorldObject*>(pListenable);

		if (pListenableDependent == this->m_pTarget)
		{
			this->m_pTarget->instance_released();
			this->m_pTarget = nullptr;
		}
		else
		{
			throw SGException("Received notification from unexpected object");
		}

		break;
	default:
		throw SGException("Received unexpected notification");
	}
}

void CChaseCamera::movement_update(float const flDelta)
{
	if(this->m_pTarget == nullptr)
	{
		return;
	}

	Vector2f vTargetPosition = this->m_pTarget->get_position();

	this->m_vPosition = CChaseCamera::chase_interpolate(this->m_vPosition, vTargetPosition, 10.0f * flDelta);
}

Vector2f CChaseCamera::chase_interpolate(Vector2f const &pos, Vector2f const &to, float const flBy)
{
	Vector2f diff = to - pos;
	float dist = diff.magnitude();
	float angle = atan2f(diff.y, diff.x);
	float modifiedBy = flBy * (clamp(dist, 0.0f, 500.0f) / 1000.0f + 0.25);

	Vector2f vec;

	vec.x = modifiedBy * dist * cos(angle);
	vec.y = modifiedBy * dist * sin(angle);

	return pos + vec;
}