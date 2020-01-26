#pragma once

#include "Defs.hxx"
#include "EngineProtos.hxx"

typedef long InstanceId;
typedef unsigned int InstanceFlags;
#define INVALID_INSTANCE -1L

/*
 * All objects are assigned an instance ID by the engine.
 * The IWorldInstance interface provides access to the instance ID.
 * 
 * In addition, instance_create is invoked by the engine
 * upon creation, and instance_destroy is invoked when the object
 * is marked as dead.
 */
interface IWorldInstance
{
	enum
	{
		InstanceFlag = 1 << 0
	};

	virtual ~IWorldInstance(void) { };

	//instance_create is invoked after object allocation,
	//immediately after the constructor
	virtual void instance_create(InstanceId const) = 0;

	//instance_destroy is invoked immediately before destruction
	virtual void instance_destroy(InstanceId const) = 0;

	//return this object's instance id
	virtual InstanceId instance_get_id(void) = 0;

	//return this object's instance flags
	//instance flags should be set by instance_create and -not modified-
	virtual InstanceFlags instance_get_flags(void) = 0;
    
    //invoke to mark this instance for deletion, in which
    //case it will be deleted during the next tick
    virtual void mark_for_deletion(void) = 0;
    
    //return whether this instance is marked for deletion
    virtual bool is_marked_for_deletion(void) = 0;

	//increment reference counter
	virtual void instance_acquired(void) = 0;

	//decrement reference counter
	virtual void instance_released(void) = 0;

	//should return true if this instance's reference count
	//is at zero, or if the instance is always safely destroyable
	virtual bool is_safely_destroyable(void) = 0;
};
