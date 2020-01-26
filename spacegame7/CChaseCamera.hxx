#pragma once

#include "CSimpleCamera.hxx"
#include "IWorldObject.hxx"

/*
 * percentage which is applied during lerp
 * between camera's position and camera target's
 * position
 */
#define CHASE_CAMERA_LERP_PERCENTAGE 0.5f //0.925f

/*
 * The chase camera follows a given target.
 * Target should be set using acquire_target() and released with
 * release_target().
 */
class CChaseCamera : public CSimpleCamera, public CListener
{
public:
	CChaseCamera(void);
	~CChaseCamera(void);

	/*
	 * ChaseCamera needs a target to chase
	 */
	void acquire_target(IWorldObject *);

	/*
	 * while ChaseCamera has an object targeted,
	 * neither the ChaseCamera nor the object should be
	 * destroyed. first, release_target() should be
	 * invoked, then the ChaseCamera and its former target
	 * can be destroyed in arbitrary order
	 */
	void release_target(void);

	virtual void listen_notify(Notification const, IListenable *);
	
protected:
	virtual void movement_update(float const) final;

	static Vector2f chase_interpolate(Vector2f const &, Vector2f const &, float const);

private:
	IWorldObject * m_pTarget;
};