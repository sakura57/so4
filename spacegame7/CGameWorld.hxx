#pragma once

#include <mutex>
#include <list>
#include <random>

#include "IWorld.hxx"
#include "CInstanceFactory.hxx"
#include "SGLib.hxx"
#include "CStaticObject.hxx"

typedef std::list<IWorldInstance*> InstanceList;

class CGameWorld : public IWorld
{
public:
	virtual void enqueue_event_callback(float const, WorldEventCallback);

	virtual void instance_enqueue(IWorldInstance *) final;

	virtual void begin_world_transaction(void) final;
	virtual void end_world_transaction(void) final;

	virtual void instance_add(IWorldInstance *) final;
	virtual void instance_remove(IWorldInstance *) final;
	virtual void destroy_all_instances(void) final;

	virtual void set_player(InstanceId const) final;
	virtual InstanceId get_player(void) final;

	/*
	 * Call ONLY from world thread
	 */
	virtual InstanceId get_player_unchecked(void) final;

	virtual IWorldInstance * instance_walk_begin(void) final;
	virtual IWorldInstance * instance_walk_next(void) final;

	virtual void world_tick(float const) final;

	virtual InstanceId instantiate_character(Character const *, Vector2f const &, float const) final;

	virtual void get_all_instances(std::list<InstanceId> &) final;

	virtual InstanceId get_instance_at(Vector2f const &) final;

	virtual void shifting_out(void) final;

	//Create a static object given a static texture ID, position and rotation parameters.
	virtual void create_static(StaticTextureId const, float const, float const, float const, float const, Vector2f const &) final;

	//Return all static objects.
	virtual std::vector<CStaticObject> const &get_all_statics(void) final;

	//Release static objects
	virtual void release_statics(void) final;

private:
	struct EventCallbackTimer
	{
		float flTimeRemaining;
		WorldEventCallback fCallback;
	};

	//mutex as opposed to spinlock is chosen
	//for CGameWorld since some methods are quite
	//expensive (e.g. world_tick)

	std::mutex m_mFieldAccess;
	Spinlock m_mQueueAccess;

	InstanceId m_iPlayerId;
	InstanceList m_lInstanceQueue;
	InstanceList m_lInstances;
	InstanceList::iterator m_iInstanceWalkIterator;

	std::vector<CStaticObject> m_vStaticObjects;
	std::list<EventCallbackTimer> m_vEventTimers;

	void detect_and_resolve_collisions(IWorldObject *);
	void sort_statics(void);
};
