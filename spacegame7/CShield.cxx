#include "CShield.hxx"
#include "CShip.hxx"

CShield::CShield(void)
{
	this->m_bInitialized = false;
}

CShield::~CShield(void)
{
	//we have nothing to free
}

void CShield::initialize(CShield::CreationParameters const &parms)
{
	this->m_iParentInstanceId = parms.iParentInstanceId;
	this->m_fBlinkRemaining = -1.0f;
	this->m_uiAppearanceId = parms.uiAppearanceId;

	CPhantomObject::initialize(parms);
}

void CShield::instance_create(InstanceId const instanceId)
{
	this->m_iInstanceId = instanceId;

	this->m_uiInstanceFlags =
		IWorldInstance::InstanceFlag |
		IWorldObject::InstanceFlag |
		ILiving::InstanceFlag |
		CPhantomObject::InstanceFlag |
		CShield::InstanceFlag;

	this->m_uiReferenceCount = 0;
	this->m_bMarkedForDeletion = false;
}

Vector2f CShield::get_position(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	IWorldObject *pParent = this->get_parent();

	if (pParent)
	{
		Vector2f vParentPosition = pParent->get_position();

		return vParentPosition;
	}

	return Vector2f(0.0f, 0.0f);
}

void CShield::set_position(Vector2f const &newPosition)
{
	//do nothing
}

float CShield::get_rotation(void)
{
	//shields always have the same rotation
	//can change this later, e.g. generate random
	return 0.0f;
}

void CShield::set_rotation(float const newRotation)
{
	//do nothing
}

AppearanceId CShield::get_appearance()
{
	SCOPE_LOCK(this->m_mFieldAccess);

	CShip * pShip = static_cast<CShip*>(this->get_parent());

	if(!pShip)
	{
		return APPEARANCE_NONE;
	}

	if(pShip->shields_up() && this->m_fBlinkRemaining > 0.0f)
	{
		return this->m_uiAppearanceId;
	}

	return APPEARANCE_NONE;
}

void CShield::set_appearance(AppearanceId const newAppearance)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_uiAppearanceId = newAppearance;
}

void CShield::alive_tick(float const fDelta)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_fBlinkRemaining -= fDelta;
}

void CShield::alive_set(bool const)
{
	//do nothing; shield objects are always alive
}
