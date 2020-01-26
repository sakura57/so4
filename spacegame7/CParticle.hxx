#pragma once

#include "Util.hxx"

typedef unsigned int AnimationId;

class CParticle
{
public:
	CParticle();
	CParticle(Vector2f const &, Vector2f const &, AnimationId const, float const, float const);
	~CParticle();

	Vector2f get_position(void);
	void update(float const);

	AnimationId get_animation_id(void)
	{
		return this->iGfxId;
	};

	float get_elapsed_time(void)
	{
		return this->timeElapsed;
	};

	float get_rotation(void)
	{
		return this->rotation;
	};

private:
	AnimationId iGfxId;
	Vector2f vel;
	Vector2f pos;
	float angular_vel;
	float timeElapsed;
	float rotation;
};