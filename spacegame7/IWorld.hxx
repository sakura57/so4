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

#include <functional>

#include "Defs.hxx"
#include "IRenderPipeline.hxx"
#include "IWorldObject.hxx"
#include "Character.hxx"
#include "ITransientStateStructure.hxx"
#include "IStaticTextureManager.hxx"
#include "CStaticObject.hxx"

typedef std::function<void(IWorld *)> WorldEventCallback;

/*
 * IWorld interface represents the game world
 * containing world objects.
 */
interface IWorld : public ITransientStateStructure
{
	virtual void enqueue_event_callback(float const, WorldEventCallback) = 0;

	/*
	 * Enqueue the instance such that it is added to the
	 * instance list on the next tick
	 */
	virtual void instance_enqueue(IWorldInstance *) = 0;

	virtual void begin_world_transaction(void) = 0;
	virtual void end_world_transaction(void) = 0;

	/*
	 * Add the instance immediately to the instance list
	 * instance_enqueue should be preferred in most
	 * situations.
	 */
	virtual void instance_add(IWorldInstance *) = 0;
	virtual void instance_remove(IWorldInstance *) = 0;
	virtual void destroy_all_instances(void) = 0;

	virtual void set_player(InstanceId const) = 0;
	virtual InstanceId get_player(void) = 0;

	virtual InstanceId get_player_unchecked(void) = 0;

	virtual IWorldInstance * instance_walk_begin(void) = 0;
	virtual IWorldInstance * instance_walk_next(void) = 0;

	virtual InstanceId instantiate_character(Character const *, Vector2f const &, float const) = 0;

	//ticks ILiving, IPhysicalObject
	virtual void world_tick(float const) = 0;

	//DO NOT CALL from outside the world thread.
	virtual void get_all_instances(std::list<InstanceId> &) = 0;

	virtual InstanceId get_instance_at(Vector2f const &) = 0;

	//Create a static object given a static texture ID, position and rotation parameters.
	virtual void create_static(StaticTextureId const, float const, float const, float const, float const, Vector2f const &) = 0;

	/*
	 * Return a ref to the vector with all static objects. Note that this will potentially lock a mutex,
	 * the client should call release_statics() after they are finished.
	 */
	virtual std::vector<CStaticObject> const &get_all_statics(void) = 0;

	/*
	 * Always call after get_all_statics() is called
	 */
	virtual void release_statics(void) = 0;
private:
	InstanceId m_iPlayerId;
};