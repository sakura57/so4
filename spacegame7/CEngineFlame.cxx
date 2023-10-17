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
#include "CEngineFlame.hxx"

CEngineFlame::CEngineFlame(void)
{
	this->m_bInitialized = false;
}

CEngineFlame::~CEngineFlame(void)
{
	//we have nothing to free
}

void CEngineFlame::initialize(CEngineFlame::CreationParameters const &parms)
{
	this->m_iParentInstanceId = parms.iParentInstanceId;
	this->m_uiAppearanceId = parms.uiAppearanceId;
	this->m_bInitialized = true;
}

void CEngineFlame::instance_create(InstanceId const instanceId)
{
	this->m_iInstanceId = instanceId;

	this->m_uiInstanceFlags =
		IWorldInstance::InstanceFlag |
		IWorldObject::InstanceFlag |
		CPhantomObject::InstanceFlag |
		CEngineFlame::InstanceFlag;

	this->m_uiReferenceCount = 0;
	this->m_bMarkedForDeletion = false;
}

Vector2f CEngineFlame::get_position(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	IWorldObject *pParent = this->get_parent();

	if(pParent)
	{
		Vector2f vParentPosition = pParent->get_position();

		vParentPosition += Matrix2f::rotation_matrix(pParent->get_rotation()) * Vector2f(-48.0f, 0.0f);

		return vParentPosition;
	}

	return Vector2f(0.0f, 0.0f);
}

void CEngineFlame::set_position(Vector2f const &newPosition)
{
	//do nothing
}

float CEngineFlame::get_rotation(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	IWorldObject *pParent = this->get_parent();

	if(pParent)
	{
		float vParentRotation = pParent->get_rotation();

		return vParentRotation;
	}

	return 0.0f;
}

void CEngineFlame::set_rotation(float const newRotation)
{
	//do nothing
}

AppearanceId CEngineFlame::get_appearance()
{
	SCOPE_LOCK(this->m_mFieldAccess);

	CShip * pShip = static_cast<CShip*>(this->get_parent());

	if(!pShip)
	{
		return APPEARANCE_NONE;
	}

	if(pShip->get_throttle() > 0.0f)
	{
		return this->m_uiAppearanceId;
	}

	return APPEARANCE_NONE;
}

void CEngineFlame::set_appearance(AppearanceId const newAppearance)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_uiAppearanceId = newAppearance;
}