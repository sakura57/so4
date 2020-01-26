#include "CStaticCamera.hxx"

CStaticCamera::CStaticCamera(void)
	: m_vTarget(0.0f, 0.0f)
{
}

CStaticCamera::~CStaticCamera(void)
{
}

void CStaticCamera::set_target_point(Vector2f const &tv)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vTarget = tv;
	this->m_vPosition = tv;
}

void CStaticCamera::movement_update(float const)
{
	//do nothing; our position is already set by set_target_point
}
