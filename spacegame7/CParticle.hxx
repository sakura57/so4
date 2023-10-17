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