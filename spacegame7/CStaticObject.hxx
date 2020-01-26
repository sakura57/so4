#pragma once

#include "IStaticTextureManager.hxx"

class CStaticObject
{
public:
	CStaticObject(StaticTextureId const);
	CStaticObject(StaticTextureId const, float const, float const, float const, float const, Vector2f const &);
	~CStaticObject();

	StaticTextureId get_texture_id(void) const;

	void set_position(float const, float const, float const);
	void get_position(float &, float &, float &) const;

	void set_rotation(float const);
	void get_rotation(float &) const;

	void set_bounds(Vector2f const &);
	void get_bounds(Vector2f &) const;

private:
	StaticTextureId m_iStaticTextureId;
	float m_flPositionX, m_flPositionY, m_flPositionZ;
	float m_flRotation;
	Vector2f m_vBounds;
};