#pragma once

#include <random>
#include "Archetype.hxx"
#include "CNonPhysical.hxx"
#include "EngineProtos.hxx"
#include "Defs.hxx"
#include "ILiving.hxx"
#include "Pilot.hxx"
#include "ICharacterEntity.hxx"

#define TARGET_ACQUISITION_DISTANCE 3000.0f
#define TARGETING_ANGLE_THRESHOLD deg2rad(5.0f)

/*
* Class representing the world instance
* of an AI controller. Behavior is dictated by
* its Pilot.
*/
class CAIController : public CNonPhysical, public virtual ILiving
{
public:
	enum
	{
		InstanceFlag = 1 << 16
	};

	enum class State
	{
		Idle,
		Engaging,
		Goto,
		Stabilize
	};

	struct CreationParameters
	{
		Pilot const *pPilot;
		InstanceId iParent;
		ICharacterEntity *pParentEntity;
	};

	CAIController(void)
	{
		//nothing to do
	};

	virtual ~CAIController();

	virtual void CAIController::instance_create(InstanceId const instanceId)
	{
		this->m_iInstanceId = instanceId;

		this->m_uiInstanceFlags =
			ILiving::InstanceFlag |
			IWorldInstance::InstanceFlag |
			CNonPhysical::InstanceFlag |
			CAIController::InstanceFlag;

		this->m_uiReferenceCount = 0;
		this->m_bMarkedForDeletion = false;
	}

	virtual void initialize(CreationParameters &creationParms);

	virtual void alive_set(bool const bAlive) final
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		this->m_bAlive = bAlive;
	};

	virtual bool is_alive(void) final
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_bAlive;
	};

	virtual void alive_tick(float const flTick) final;

	virtual Pilot const * get_pilot(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		return this->m_pPilot;
	};

	virtual void goto_directive(Vector2f const &vGotoPoint)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		this->m_vGotoPoint = vGotoPoint;
		this->m_state = State::Goto;
	};

	virtual void chill_directive(void)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		this->m_state = State::Idle;
	};

	static void aim_at_point(CShip *, Vector2f const &);

	static int get_num_controllers_targeting_player(void);

protected:
	InstanceId m_iParent;
	Pilot const *m_pPilot;
	bool m_bAlive;
	State m_state;
	ICharacterEntity *m_pParentEntity;
	std::vector<SpellId> m_vAvailableSpells;

	float m_fTimeToAcquireNextTarget;
	float m_fBurstTimer;
	float m_fBreakoffTimer;

	Vector2f m_vGotoPoint;

	InstanceId m_iCurrentTarget;

private:
	void acquire_next_target(CShip *);

	Vector2f get_target_lead_fire_position(CEquippedObject *, CEquippedObject *);

	void do_engage(CShip *, CEquippedObject *, float const);
	void do_idle(CShip *, float const);
	void do_stabilize(CShip *, float const);
	void do_goto(CShip *, float const);

	int has_spell(SpellId const);

	bool m_bTargetingPlayer;

	static int iNumControllersTargetingPlayer;
	static Spinlock sSharedAccess;
};
