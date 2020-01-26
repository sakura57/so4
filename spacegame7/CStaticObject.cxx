#include "CStaticObject.hxx"

CStaticObject::CStaticObject(StaticTextureId const iStaticTextureId)
{
	this->m_iStaticTextureId = iStaticTextureId;
}

CStaticObject::CStaticObject(StaticTextureId const iStaticTextureId,
	float const flPositionX, float const flPositionY, float const flPositionZ,
	float const flRotation,
	Vector2f const &vBounds)
	: m_iStaticTextureId(iStaticTextureId),
	m_flPositionX(flPositionX),
	m_flPositionY(flPositionY),
	m_flPositionZ(flPositionZ),
	m_flRotation(flRotation),
	m_vBounds(vBounds)
{

}

CStaticObject::~CStaticObject()
{
	
}

StaticTextureId CStaticObject::get_texture_id(void) const
{
	return this->m_iStaticTextureId;
}

void CStaticObject::set_position(float const flPositionX, float const flPositionY, float const flPositionZ)
{
	this->m_flPositionX = flPositionX;
	this->m_flPositionY = flPositionY;
	this->m_flPositionZ = flPositionZ;
}

void CStaticObject::get_position(float &flPositionX, float &flPositionY, float &flPositionZ) const
{
	flPositionX = this->m_flPositionX;
	flPositionY = this->m_flPositionY;
	flPositionZ = this->m_flPositionZ;
}

void CStaticObject::set_rotation(float const flRotation)
{
	this->m_flRotation = flRotation;
}

void CStaticObject::get_rotation(float &flRotation) const
{
	flRotation = this->m_flRotation;
}

void CStaticObject::set_bounds(Vector2f const &vBounds)
{
	this->m_vBounds = vBounds;
}

void CStaticObject::get_bounds(Vector2f &vBounds) const
{
	vBounds = this->m_vBounds;
}