#include "CParticle.hxx"

CParticle::CParticle()
{
	this->timeElapsed = 0;
	this->pos.x = 0.0;
	this->pos.y = 0.0;
	this->iGfxId = 7;
}

CParticle::CParticle(Vector2f const &mypos, Vector2f const &myvel, AnimationId const my_gfx_id, float const fRotation, float const fAngularVel)
{
	this->timeElapsed = 0;
	this->pos.x = mypos.x;
	this->pos.y = mypos.y;
	this->vel.x = myvel.x;
	this->vel.y = myvel.y;
	this->iGfxId = my_gfx_id;
	this->rotation = fRotation;
	this->angular_vel = fAngularVel;
}

CParticle::~CParticle()
{
	//dummy func
}

void CParticle::update(float const fDelta)
{
	this->timeElapsed += fDelta;
	this->pos.x += this->vel.x * fDelta;
	this->pos.y += this->vel.y * fDelta;

	this->rotation += this->angular_vel;
}

Vector2f CParticle::get_position(void)
{
	return this->pos;
}