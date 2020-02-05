#pragma once

#include "CSimpleCamera.hxx"

/*
 * The vignette camera begins focused on a given point, but moves
 * along a linear path towards an end point.
 */
class CVignetteCamera : public CSimpleCamera
{
public:
	CVignetteCamera(void);
	~CVignetteCamera(void);

	/*
	 * Vignette camera needs a point to focus on
	 */
	void set_origin_point(Vector2f const&);

	/*
	 *  The point the vignette camera will end on
	 */
	void set_final_point(Vector2f const&);

	/*
	 * Set the time duration of the movement along the path
	 */
	void set_path_duration(float const);

protected:
	virtual void movement_update(float const) final;

private:
	Vector2f m_vStartPoint;
	Vector2f m_vEndPoint;
	float m_flTimeElapsed;
	float m_flPathDuration;
};