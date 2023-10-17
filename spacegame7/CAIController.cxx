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
#include "CAIController.hxx"
#include "CEquippedObject.hxx"
#include "CShip.hxx"
#include <algorithm>
#include "Spells.hxx"

#define TARGET_REACQUISITION_TIME_DEVIATION 0.25f

#define PERSONAL_THROTTLE_DEVIATION 0.1f

Spinlock CAIController::sSharedAccess;
int CAIController::iNumControllersTargetingPlayer = 0;

CAIController::~CAIController()
{
	if(this->m_bTargetingPlayer)
	{
		this->m_bTargetingPlayer = false;

		CAIController::sSharedAccess.lock();

		--CAIController::iNumControllersTargetingPlayer;

		CAIController::sSharedAccess.unlock();
	}
}

void CAIController::initialize(CreationParameters & creationParms)
{
	this->m_pPilot = creationParms.pPilot;
	this->m_iParent = creationParms.iParent;
	this->m_bAlive = true;

	this->m_state = State::Idle;
	this->m_iCurrentTarget = INVALID_INSTANCE;
	this->m_fTimeToAcquireNextTarget = -1.0f;

	this->m_fBurstTimer = -1.0f;
	this->m_fBreakoffTimer = -1.0f;
	this->m_vGotoPoint = Vector2f(0.0f, 0.0f);
	this->m_pParentEntity = creationParms.pParentEntity;

	this->m_bTargetingPlayer = false;

	if(this->m_pParentEntity != nullptr)
	{
		this->m_vAvailableSpells = this->m_pParentEntity->get_mapped_spells();
	}

	//find an amount (unique to each character) to be subtracted from the throttle percent
	{
		std::uniform_real_distribution<float> throttleDist(0.0f, PERSONAL_THROTTLE_DEVIATION);

		this->m_fPersonalThrottlePercent = throttleDist(SG::get_random());
	}
}

void CAIController::alive_tick(float const flDelta)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(this->m_iParent == INVALID_INSTANCE)
	{
		return;
	}

	//our parent must be a CEquippedObject, there is no other possibility.
	//this will always succeed
	//TODO: add an is_allocated check before this
	CEquippedObject *pParentObj = static_cast<CEquippedObject*>(SG::get_engine()->instance_get(this->m_iParent));

	//but just in case...
	if (pParentObj == nullptr || !pParentObj->is_alive())
	{
		return;
	}

	//this may be null, maybe not.
	CShip *pParentShip = nullptr;

	if (pParentObj->instance_get_flags() & CShip::InstanceFlag)
	{
		pParentShip = static_cast<CShip*>(pParentObj);
	}
	else
	{
		//If we are not a CShip, then do not handle movement or 
		return;
	}

	//in combat or out of combat ability usage - handle stuff like healing spells here, etc
	//determine if we should heal (only heal below 60% health with nano stream, 40% for nano swarm)
	if(this->m_pParentEntity)
	{
		int iNanoStreamSlot = this->has_spell(SPELL_NANOBOT_STREAM);
		int iNanoSwarmSlot = this->has_spell(SPELL_NANOBOT_SWARM);

		if(iNanoStreamSlot != -1)
		{
			if(pParentShip->get_relative_health() < 0.6f)
			{
				if(this->m_pParentEntity->can_cast_spell(iNanoStreamSlot))
				{
					this->m_pParentEntity->cast_spell(iNanoStreamSlot, this->m_iParent, INVALID_INSTANCE);
				}
			}
		}

		if(iNanoSwarmSlot != -1)
		{
			if(pParentShip->get_relative_health() < 0.4f)
			{
				if(this->m_pParentEntity->can_cast_spell(iNanoSwarmSlot))
				{
					this->m_pParentEntity->cast_spell(iNanoSwarmSlot, this->m_iParent, INVALID_INSTANCE);
				}
			}
		}
	}

	//baseline 0 throttle
	pParentShip->set_throttle(0.0f);
	pParentShip->set_spin_throttle(0.0f);

	//handle targeting
	this->m_fTimeToAcquireNextTarget -= flDelta;

	if(this->m_state != State::Stabilize && this->m_fTimeToAcquireNextTarget < 0.0f)
	{
		this->acquire_next_target(pParentShip);

		std::uniform_real_distribution<float> deviationDist(-TARGET_REACQUISITION_TIME_DEVIATION / 2, TARGET_REACQUISITION_TIME_DEVIATION / 2);

		this->m_fTimeToAcquireNextTarget = this->m_pPilot->flTargetReacquisitionTime + deviationDist(SG::get_random());
	}

	//our parent must be a CEquippedObject, there is no other possibility.
	//this will always succeed
	CShip *pTargetShip = nullptr;

	if(this->m_iCurrentTarget != INVALID_INSTANCE)
	{
		if(SG::get_engine()->instance_is_allocated(this->m_iCurrentTarget))
		{
			pTargetShip = static_cast<CShip*>(SG::get_engine()->instance_get(this->m_iCurrentTarget));

			if (pTargetShip == nullptr || !pTargetShip->is_alive())
			{
				this->m_iCurrentTarget = INVALID_INSTANCE;
				pTargetShip = nullptr;
			}
		}
		else
		{
			this->m_iCurrentTarget = INVALID_INSTANCE;
		}
	}

	switch(this->m_state)
	{
	case State::Idle:
		this->do_idle(pParentShip, flDelta);
		break;
	case State::Engaging:
		this->do_engage(pParentShip, pTargetShip, flDelta);
		break;
	case State::Goto:
		this->do_goto(pParentShip, flDelta);
		break;
	case State::Stabilize:
		this->do_stabilize(pParentShip, flDelta);
		break;
	default:
		break;
	}
}

void CAIController::acquire_next_target(CShip *pParentObj)
{
	IWorld *pGameWorld = SG::get_world();
	IEngine *pGameEngine = SG::get_engine();

	std::list<InstanceId> worldInstances;
	pGameWorld->get_all_instances(worldInstances);

	float flClosestTargetDistance = 999999.0f;
	InstanceId targetCandidate = INVALID_INSTANCE;
	bool bCandidateIsPlayer = false;

	for(InstanceId instanceId : worldInstances)
	{
		//don't target ourselves
		if(instanceId == this->m_iParent)
		{
			continue;
		}

		IWorldInstance *pInstance = pGameEngine->instance_get(instanceId);

		if(pInstance->instance_get_flags() & CShip::InstanceFlag)
		{
			CShip *pShip = static_cast<CShip *>(pInstance);

			Vector2f pTargetPosition = pShip->get_position();
			Vector2f pParentObjPosition = pParentObj->get_position();

			float flDistance = (pTargetPosition - pParentObjPosition).magnitude();

			if(flDistance > TARGET_ACQUISITION_DISTANCE)
			{
				continue;
			}

			//ignore aggressors of the same faction
			{
				Faction const* pAggressorFaction = pShip->get_faction();
				Faction const* pFaction = pParentObj->get_faction();

				if (
					(pAggressorFaction != nullptr && pFaction != nullptr) &&
					(pAggressorFaction->id == pFaction->id)
					)
				{
					continue;
				}
			}

			CAttitudeSet ourAttitudes = pParentObj->get_attitude_set();
			float flAttitudeToTarget = ourAttitudes.get_attitude(instanceId);

			if(!(flAttitudeToTarget <= THRESHOLD_HOSTILE))
			{
				continue;
			}

			if(flDistance < flClosestTargetDistance)
			{
				flClosestTargetDistance = flDistance;
				targetCandidate = instanceId;
				bCandidateIsPlayer = pShip->is_player();
			}
		}
	}

	if(targetCandidate != INVALID_INSTANCE)
	{
		if(bCandidateIsPlayer && targetCandidate != this->m_iCurrentTarget)
		{
			if(!this->m_bTargetingPlayer)
			{
				CAIController::sSharedAccess.lock();

				++CAIController::iNumControllersTargetingPlayer;

				CAIController::sSharedAccess.unlock();

				this->m_bTargetingPlayer = true;
			}
		}

		this->m_state = State::Engaging;
		this->m_iCurrentTarget = targetCandidate;
		pParentObj->set_target(this->m_iCurrentTarget);

		return;
	}

	//this point is reached if no target was acquired
	if (this->m_state == State::Engaging)
	{
		//return to idle state
		this->m_state = State::Idle;

		pParentObj->set_target(INVALID_INSTANCE);

		if(this->m_bTargetingPlayer)
		{
			CAIController::sSharedAccess.lock();

			--CAIController::iNumControllersTargetingPlayer;

			CAIController::sSharedAccess.unlock();

			this->m_bTargetingPlayer = false;
		}
	}
}

void CAIController::aim_at_point(CShip *pShip, Vector2f const &vTargetPoint)
{
	if(pShip == nullptr)
	{
		return;
	}

	Vector2f vPosition = pShip->get_position();
	
	float flRotation = normalize_angle_rad(deg2rad(pShip->get_rotation()));
	float flDesiredRotation = look_at_angle(vPosition, vTargetPoint) - M_PI;
	
	float k = 500.0f;
	float m = 1.0f;
	float d = 3.5f;
	float c = -1.0f * sqrt(4.0f * m * k) * d;
	float v = deg2rad(pShip->get_angular_velocity());
	float flDiff = normalize_angle_rad(angle_diff(flRotation, flDesiredRotation));
	float x = M_PI - flDiff;

	float flMax = abs(k * M_PI + c * M_PI);

	float flSpringForce = k * x;
	float flDampingForce = c * v;

	float fNet = (flSpringForce + flDampingForce) / 50.0f;

	pShip->set_spin_throttle(fNet);
}

Vector2f CAIController::get_target_lead_fire_position(CEquippedObject *pParent, CEquippedObject *pTarget)
{
	Vector2f vParentPosition = pParent->get_position();
	Vector2f vTargetPosition = pTarget->get_position();
	Vector2f vParentVelocity = pParent->get_velocity();
	Vector2f vTargetVelocity = pTarget->get_velocity();
	float flParentRotation = pParent->get_rotation();

	float flAverageMuzzleVelocity = pParent->get_average_weapon_muzzle_velocity();
	Vector2f vPositionDelta = vTargetPosition - vParentPosition;
	float flTravelDistance = vPositionDelta.magnitude();
	float flAverageTravelTime = flTravelDistance / flAverageMuzzleVelocity;

	Vector2f vPredictedTargetPosition = vTargetPosition + (flAverageTravelTime * vTargetVelocity) - (flAverageTravelTime * vParentVelocity);
	return vPredictedTargetPosition;
}

void CAIController::do_engage(CShip *pParentShip, CEquippedObject *pTargetShip, float flDelta)
{
	if (pTargetShip == nullptr)
	{
		return;
	}

	//in combat or out of combat ability usage - handle stuff like healing spells here, etc
	//determine if we should heal (only heal below 60% health with nano stream, 40% for nano swarm)
	if(this->m_pParentEntity)
	{
		int iMissileSwarmSlot = this->has_spell(SPELL_MISSILE_SWARM_WEAK);

		if(iMissileSwarmSlot != -1)
		{
			if(pTargetShip->instance_get_flags() | CShip::InstanceFlag)
			{
				CShip *pTargetAsShip = static_cast<CShip *>(pTargetShip);

				if(!pTargetAsShip->shields_up())
				{
					if(this->m_pParentEntity->can_cast_spell(iMissileSwarmSlot))
					{
						this->m_pParentEntity->cast_spell(iMissileSwarmSlot, this->m_iParent, this->m_iCurrentTarget);
					}
				}
			}
		}
	}

	Vector2f vTargetPos = pTargetShip->get_position();
	Vector2f vParentPos = pParentShip->get_position();

	float flTargetDistance = (vTargetPos - vParentPos).magnitude();

	Vector2f vPredictedTargetPosition = (1.5f * (this->get_target_lead_fire_position(pParentShip, pTargetShip) - vTargetPos)) + vTargetPos;
	//Vector2f vPredictedTargetPosition = this->get_target_lead_fire_position(pParentShip, pTargetShip);

	CAIController::aim_at_point(pParentShip, vPredictedTargetPosition);

	float flNeededAngle = look_at_angle(vParentPos, vPredictedTargetPosition);
	float flCurrentAngle = normalize_angle(deg2rad(pParentShip->get_rotation()));

	float flDeltaAngle = rad2deg(abs(angle_diff(flNeededAngle, flCurrentAngle)));

	//if the target is within our firing cone and our range, run a firing burst
	if (flTargetDistance < pParentShip->get_max_weapon_range())
	{
		if (flDeltaAngle < this->m_pPilot->flFiringCone)
		{
			this->m_fBurstTimer -= flDelta;

			if (this->m_fBurstTimer < 0.0f)
			{
				this->m_fBurstTimer = this->m_pPilot->flFiringBurstDelay + this->m_pPilot->flFiringBurstDuration;

				pParentShip->fire_weapons_if_ready(ARCH_MISSILE);
			}
			else if (this->m_fBurstTimer < this->m_pPilot->flFiringBurstDuration)
			{
				pParentShip->fire_weapons_if_ready(ARCH_WEAPON);
			}
		}
	}

	this->m_fBreakoffTimer -= flDelta;

	if (this->m_fBreakoffTimer < 0.0f)
	{
		this->m_fBreakoffTimer = this->m_pPilot->flBreakoffDelay + this->m_pPilot->flBreakoffDuration;
	}
	else if (this->m_fBreakoffTimer < this->m_pPilot->flBreakoffDuration)
	{
		Vector2f vNormalizedDelta = (vTargetPos - vParentPos).normalize();
		Vector2f vNormalizedVelocity = pParentShip->get_velocity().normalize();

		//only stabilize if we are not heading towards our target
		if(vNormalizedVelocity.dot(vNormalizedDelta) < 0.01f)
		{
			this->m_state = State::Stabilize;
		}

		this->m_fBreakoffTimer = -1.0f;
	}
	else
	{
		if (flDeltaAngle < this->m_pPilot->flApproachCone)
		{
			float threshold = (flTargetDistance - this->m_pPilot->flPreferredEnemyDistance) / 50.0f * (1.0f - this->m_fPersonalThrottlePercent);

			Vector2f vParentVelocity = pParentShip->get_velocity();

			float flMult = vParentVelocity.dot((vTargetPos - vParentPos).normalize());

			if(flMult < 500.0f)
			{
				pParentShip->set_throttle(clamp(threshold, -1.0f, 1.0f));
			}
		}
	}
}

void CAIController::do_idle(CShip *pParentShip, float const flDelta)
{
	//if our velocity is already low enough,
	//don't bother correcting

	Vector2f vTargetVelocity = pParentShip->get_velocity();

	Vector2f vUnit(100.0f, 0.0f);
	Vector2f vUnitVelocity = -1.0f * vTargetVelocity.normalize();
	float flRotation = atan2f(vUnitVelocity.y, vUnitVelocity.x);
	
	Vector2f vOffset = Matrix2f::rotation_matrix(rad2deg(flRotation)) * vUnit;
	Vector2f vTargetPoint = pParentShip->get_position() + vOffset;

	float flNeededAngle = look_at_angle(pParentShip->get_position(), vTargetPoint);
	float flCurrentAngle = deg2rad(normalize_angle(pParentShip->get_rotation()));

	float flDeltaAngle = rad2deg(abs(angle_diff(flNeededAngle, flCurrentAngle)));

	CAIController::aim_at_point(pParentShip, vTargetPoint);

	if (vTargetVelocity.magnitude() < 5.0f)
	{
		return;
	}

	if(flDeltaAngle < 2.5f)
	{
		pParentShip->set_throttle(1.0f);
	}
}

void CAIController::do_stabilize(CShip *pParentShip, float const flDelta)
{
	//if our velocity is already low enough,
	//don't bother correcting

	Vector2f vTargetVelocity = pParentShip->get_velocity();

	Vector2f vUnit(100.0f, 0.0f);
	Vector2f vUnitVelocity = -1.0f * vTargetVelocity.normalize();
	float flRotation = atan2f(vUnitVelocity.y, vUnitVelocity.x);

	Vector2f vOffset = Matrix2f::rotation_matrix(rad2deg(flRotation)) * vUnit;
	Vector2f vTargetPoint = pParentShip->get_position() + vOffset;

	float flNeededAngle = look_at_angle(pParentShip->get_position(), vTargetPoint);
	float flCurrentAngle = deg2rad(normalize_angle(pParentShip->get_rotation()));

	float flDeltaAngle = rad2deg(abs(angle_diff(flNeededAngle, flCurrentAngle)));

	CAIController::aim_at_point(pParentShip, vTargetPoint);

	if (vTargetVelocity.magnitude() < 250.0f)
	{
		this->m_state = State::Idle;
		return;
	}

	if (flDeltaAngle < 1.0f)
	{
		pParentShip->set_throttle(1.0f);
	}
}

void CAIController::do_goto(CShip *pParentShip, float const flDelta)
{
	//are we already there?
	Vector2f vParentPosition = pParentShip->get_position();
	Vector2f vDelta = this->m_vGotoPoint - vParentPosition;

	float flRotation = pParentShip->get_rotation();

	Vector2f vTargetPoint = this->m_vGotoPoint;

	float flNeededAngle = look_at_angle(pParentShip->get_position(), vTargetPoint);
	float flCurrentAngle = deg2rad(normalize_angle(flRotation));

	float flDeltaAngle = rad2deg(abs(angle_diff(flNeededAngle, flCurrentAngle)));

	CAIController::aim_at_point(pParentShip, vTargetPoint);

	Vector2f vNormalDelta = vDelta.normalize();
	Vector2f vParentVelocity = pParentShip->get_velocity();

	float flMult = vParentVelocity.dot(vNormalDelta);

	if (vDelta.magnitude() < 500.0f)
	{
		this->m_state = State::Idle;
		return;
	}

	if(flDeltaAngle < 5.0f && flMult < 500.0f)
	{
		pParentShip->set_throttle(1.0f);
	}
}

int CAIController::has_spell(SpellId const iSpellToFind)
{
	for(int i = 0; i < this->m_vAvailableSpells.size(); ++i)
	{
		SpellId const iSpellId = this->m_vAvailableSpells[i];
		if(iSpellId == 0) continue;
		if(iSpellId == iSpellToFind) return i;
	}

	return -1;
}

int CAIController::get_num_controllers_targeting_player()
{
	volatile int numControllersTargetingPlayer;

	CAIController::sSharedAccess.lock();

	numControllersTargetingPlayer = CAIController::iNumControllersTargetingPlayer;

	CAIController::sSharedAccess.unlock();

	return numControllersTargetingPlayer;
}