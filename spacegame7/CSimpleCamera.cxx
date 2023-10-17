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
#include "CSimpleCamera.hxx"

CSimpleCamera::CSimpleCamera(void)
{
	
}

CSimpleCamera::~CSimpleCamera(void)
{
	
}


void CSimpleCamera::instance_create(InstanceId const instanceId)
{
	this->m_iInstanceId = instanceId;

	this->m_uiInstanceFlags =
        IWorldInstance::InstanceFlag |
		ILiving::InstanceFlag |
        IWorldObject::InstanceFlag |
        ICamera::InstanceFlag |
        CSimpleCamera::InstanceFlag;
        
    this->m_bMarkedForDeletion = false;
	this->m_uiReferenceCount = 0;
}

void CSimpleCamera::instance_destroy(InstanceId const instanceId)
{
	this->listeners_send({Notification::Type::DEPENDENT_DESTROYED});

	if(this->m_iInstanceId == instanceId)
	{
		this->m_iInstanceId = -1;
	}
}

void CSimpleCamera::instance_acquired(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	++this->m_uiReferenceCount;
}

void CSimpleCamera::instance_released(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if (this->m_uiReferenceCount) --this->m_uiReferenceCount;
}

bool CSimpleCamera::is_safely_destroyable(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_uiReferenceCount == 0;
}

InstanceFlags CSimpleCamera::instance_get_flags(void)
{
	return this->m_uiInstanceFlags;
}


InstanceId CSimpleCamera::instance_get_id(void)
{
	return this->m_iInstanceId;
}

void CSimpleCamera::mark_for_deletion(void)
{
    SCOPE_LOCK(this->m_mFieldAccess);
    
    this->m_bMarkedForDeletion = true;
}

bool CSimpleCamera::is_marked_for_deletion(void)
{
    SCOPE_LOCK(this->m_mFieldAccess);
    
    return this->m_bMarkedForDeletion;
}

Vector2f CSimpleCamera::get_position(void)
{
    SCOPE_LOCK(this->m_mFieldAccess);
    
    return this->m_vPosition;
}

void CSimpleCamera::set_position(Vector2f const &vPosition)
{
    SCOPE_LOCK(this->m_mFieldAccess);
    
    this->m_vPosition = vPosition;
}

float CSimpleCamera::get_rotation(void)
{
    //cameras are not rotation-enabled
	return 0.0f;
}

void CSimpleCamera::set_rotation(float const)
{
    //cameras are not rotation-enabled
}

AppearanceId CSimpleCamera::get_appearance(void)
{
    return APPEARANCE_NONE;
}

void CSimpleCamera::set_appearance(AppearanceId const)
{
    //cameras have no appearance
}

void CSimpleCamera::alive_set(bool const bEnableCamera)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_bCameraEnabled = bEnableCamera;
}

void CSimpleCamera::alive_tick(float const flDelta)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(!this->m_bCameraEnabled)
	{
		return;
	}

	this->movement_update(flDelta);
}

float CSimpleCamera::get_zoom_factor(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_flZoomFactor;
}

void CSimpleCamera::set_zoom_factor(float const flZoomFactor)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vWorldBounds = flZoomFactor * this->m_vScreenDimensions;

	//update the actual zoom factor value
	this->update_zoom_factor();
}

Vector2f CSimpleCamera::get_bounds(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_vWorldBounds;
}

void CSimpleCamera::set_bounds(Vector2f const &vWorldBounds)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vWorldBounds = vWorldBounds;
    
    this->update_zoom_factor();
}

Vector2f CSimpleCamera::get_screen_dimensions(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_vScreenDimensions;
}

void CSimpleCamera::set_screen_dimensions(Vector2f const &vScreenDimensions)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_vScreenDimensions = vScreenDimensions;
    
    this->update_zoom_factor();
}

void CSimpleCamera::update_zoom_factor(void)
{
	this->m_flZoomFactor = this->m_vWorldBounds.x / this->m_vScreenDimensions.x;
}

bool CSimpleCamera::project(Vector2f const &vWorldPosition, Vector2f &vScreenPosition)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	Vector2f vFixedScreenDims(this->m_vScreenDimensions);
	vFixedScreenDims.y *= -1.0f;

	vScreenPosition = vWorldPosition - this->get_position() + vFixedScreenDims;

	//divide by the zoom factor to obtain real screen position
	vScreenPosition /= this->m_flZoomFactor;

	return vScreenPosition.x < this->m_vScreenDimensions.x &&
		vScreenPosition.y < this->m_vScreenDimensions.y &&
		vScreenPosition.x >= 0.0f;
}

bool CSimpleCamera::is_initialized(void)
{
	return true;
}
