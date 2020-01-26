#pragma once

#include <string>

typedef unsigned int PilotId;

struct Pilot
{
	PilotId id;
	std::string name;

	float flFiringCone;
	float flFiringBurstDuration;
	float flFiringBurstDelay;
	float flPreferredEnemyDistance;
	float flTargetReacquisitionTime;
	float flApproachCone;
	float flBreakoffDuration;
	float flBreakoffDelay;
};