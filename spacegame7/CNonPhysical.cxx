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
#include "CNonPhysical.hxx"


CNonPhysical::CNonPhysical(void)
{
}

CNonPhysical::~CNonPhysical(void)
{
	//we have nothing to free
}

void CNonPhysical::instance_create(InstanceId const instanceId)
{
	this->m_iInstanceId = instanceId;

	this->m_uiInstanceFlags =
		IWorldInstance::InstanceFlag |
		CNonPhysical::InstanceFlag;

	this->m_uiReferenceCount = 0;
	this->m_bMarkedForDeletion = false;
}

void CNonPhysical::instance_destroy(InstanceId const instanceId)
{
	if (this->m_iInstanceId == instanceId)
	{
		this->m_iInstanceId = -1;
	}
}

InstanceFlags CNonPhysical::instance_get_flags(void)
{
	return this->m_uiInstanceFlags;
}


InstanceId CNonPhysical::instance_get_id(void)
{
	return this->m_iInstanceId;
}

void CNonPhysical::mark_for_deletion(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_bMarkedForDeletion = true;
}

bool CNonPhysical::is_marked_for_deletion(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_bMarkedForDeletion;
}

void CNonPhysical::instance_acquired(void)
{
	//SCOPE_LOCK(this->m_mFieldAccess);

	++this->m_uiReferenceCount;
}

void CNonPhysical::instance_released(void)
{
	//SCOPE_LOCK(this->m_mFieldAccess);

	if (this->m_uiReferenceCount)--this->m_uiReferenceCount;
}

bool CNonPhysical::is_safely_destroyable(void)
{
	//SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_uiReferenceCount == 0;
}

void CNonPhysical::initialize(CreationParameters const &creationParms)
{
	//nothing to do. CNonPhysical::CreationParameters is an empty class
}
