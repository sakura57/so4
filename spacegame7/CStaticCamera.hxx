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