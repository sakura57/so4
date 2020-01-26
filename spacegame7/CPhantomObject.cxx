#include "CPhantomObject.hxx"

CPhantomObject::CPhantomObject(void)
{
	this->m_bInitialized = false;
}

CPhantomObject::~CPhantomObject(void)
{
	//we have nothing to free
}

void CPhantomObject::initialize(CPhantomObject::CreationParameters const &parms)
{
	this->m_uiAppearanceId = parms.uiAppearanceId;

	this->m_bInitialized = true;
}

void CPhantomObject::instance_create(InstanceId const instanceId)
{
	this->m_iInstanceId = instanceId;

	this->m_uiInstanceFlags =
		IWorldInstance::InstanceFlag |
		IWorldObject::InstanceFlag |
		CPhantomObject::InstanceFlag;

	this->m_uiReferenceCount = 0;
	this->m_bMarkedForDeletion = false;
}

void CPhantomObject::instance_destroy(InstanceId const instanceId)
{
	if (this->m_iInstanceId == instanceId)
	{
		this->m_iInstanceId = -1;
	}
}

InstanceFlags CPhantomObject::instance_get_flags(void)
{
	return this->m_uiInstanceFlags;
}


InstanceId CPhantomObject::instance_get_id(void)
{
	return this->m_iInstanceId;
}

void CPhantomObject::mark_for_deletion(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_bMarkedForDeletion = true;
}

bool CPhantomObject::is_marked_for_deletion(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_bMarkedForDeletion;
}

void CPhantomObject::instance_acquired(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	++this->m_uiReferenceCount;
}

void CPhantomObject::instance_released(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if (this->m_uiReferenceCount)--this->m_uiReferenceCount;
}

bool CPhantomObject::is_safely_destroyable(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_uiReferenceCount == 0;
}

Vector2f CPhantomObject::get_position(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return Vector2f(0.0f, 0.0f);
}

void CPhantomObject::set_position(Vector2f const &newPosition)
{
	//do nothing
}

float CPhantomObject::get_rotation(void)
{
	//shields always have the same rotation
	//can change this later, e.g. generate random
	return 0.0f;
}

void CPhantomObject::set_rotation(float const newRotation)
{
	//do nothing
}

AppearanceId CPhantomObject::get_appearance()
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return APPEARANCE_NONE;
}

void CPhantomObject::set_appearance(AppearanceId const newAppearance)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_uiAppearanceId = newAppearance;
}

bool CPhantomObject::is_initialized(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_bInitialized;
}