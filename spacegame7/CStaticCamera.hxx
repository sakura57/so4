#pragma once

#include "CSimpleCamera.hxx"

/*
 * The static camera keeps its viewport centered on a given point.
 */
class CStaticCamera : public CSimpleCamera
{
public:
	CStaticCamera(void);
	~CStaticCamera(void);

	/*
	* StaticCamera needs a point to focus on
	*/
	void set_target_point(Vector2f const &);

protected:
	virtual void movement_update(float const) final;

private:
	Vector2f m_vTarget;
};