#pragma once

#include "Defs.hxx"
#include "IWorldInstance.hxx"

interface IInteractable
{
	virtual void interact(InstanceId const) = 0;
};