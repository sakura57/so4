#pragma once

#include <mutex>

#include "Defs.hxx"
#include "ICamera.hxx"
#include "ILiving.hxx"

/*
 * base abstract camera class
 * 
 * All camera types should inherit from CSimpleCamera.
 */
class CSimpleCamera : public ICamera, public ILiving
{
public:
    enum
	{
		InstanceFlag = 1 << 6
	};
    
	CSimpleCamera(void);
	virtual ~CSimpleCamera(void);
    
    //Members inherited from IWorldInstance
    virtual void instance_create(InstanceId const);
	virtual void instance_destroy(InstanceId const);
	virtual InstanceId instance_get_id(void) final;
	virtual InstanceFlags instance_get_flags(void) final;
    virtual void mark_for_deletion(void) final;
    virtual bool is_marked_for_deletion(void) final;
	virtual void instance_acquired(void) final;
	virtual void instance_released(void) final;
	virtual bool is_safely_destroyable(void) final;
    
    //Members inherited from IWorldObject
	virtual Vector2f get_position(void) final;
	virtual void set_position(Vector2f const &) final;
	virtual float get_rotation(void) final;
	virtual void set_rotation(float const) final;
	virtual AppearanceId get_appearance(void) final;
	virtual void set_appearance(AppearanceId) final;
	virtual bool is_initialized(void) final;

    //Members inherited from ICamera
	virtual Vector2f get_bounds(void);
	virtual void set_bounds(Vector2f const &);
	virtual float get_zoom_factor(void);
	virtual void set_zoom_factor(float const);
	virtual Vector2f get_screen_dimensions(void);
	virtual void set_screen_dimensions(Vector2f const &);
	virtual bool project(Vector2f const &, Vector2f &);

    
    //Members inherited from ILiving
	/*
	 * alive_tick() implementation for cameras just
	 * forwards to movement_update()
	 */
	virtual void alive_tick(float const) final;

	/*
	 * alive_set() enables or disables camera movement.
	 * for CChaseCamera, disable tracking of the player
	 * ship.
	 */
	virtual void alive_set(bool const) final;

	virtual bool is_alive(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_bCameraEnabled;
	};

protected:
	/*
	* Implementation is required. invoked by alive_tick()
	* locking of m_mFieldAccess not required, that is
    * handled by alive_tick
	*/
	virtual void movement_update(float const) = 0;

	Spinlock m_mFieldAccess;

	unsigned int m_uiReferenceCount;
    
    bool m_bMarkedForDeletion;
    InstanceId m_iInstanceId;
	InstanceFlags m_uiInstanceFlags;
    
    Vector2f m_vPosition;
	bool m_bCameraEnabled;
	Vector2f m_vScreenDimensions;
	Vector2f m_vWorldBounds;

	//more or less a cached value, since
	//set_zoom_factor should update bounds directly
	float m_flZoomFactor;

private:
	/*
	 * should be invoked internally any time screen dimensions
	 * or world bounds are modified.
	 */
	void update_zoom_factor(void);
};
