/**
 * Copyright (C) 2020 Jacob Farnsworth.
 *
 * This file is part of the Spaced Out 4 project.
 *
 * Spaced Out 4 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, version 2.
 *
 * Spaced Out 4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */
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