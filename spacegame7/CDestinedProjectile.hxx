#pragma once

#include <functional>
#include "CProjectile.hxx"

typedef std::function<void(float const, Vector2f &, float &)> DestinyFunction;

class CDestinedProjectile : public CProjectile
{
public:
	enum
	{
		InstanceFlag = 1 << 20
	};

	struct CreationParameters : public CProjectile::CreationParameters
	{
		float flProjScriptTime;
		DestinyFunction fDestinyFunction;
		bool bHasLifetimeAfterDestiny;
	};

	virtual void initialize(CreationParameters const &);

	CDestinedProjectile(void);
	virtual ~CDestinedProjectile(void);

	virtual void alive_tick(float const);
	virtual void physics_tick(float const);

	virtual void instance_create(InstanceId const instanceId)
	{
		this->m_iInstanceId = instanceId;

		this->m_uiInstanceFlags =
			ILiving::InstanceFlag |
			IWorldInstance::InstanceFlag |
			IWorldObject::InstanceFlag |
			IPhysicalObject::InstanceFlag |
			CObject::InstanceFlag |
			CProjectile::InstanceFlag |
			CDestinedProjectile::InstanceFlag;

		this->m_uiReferenceCount = 0;
		this->m_bMarkedForDeletion = false;
	};

	//virtual Vector2f get_position(void);
	//virtual float get_rotation(void);
private:
	bool m_bProjReleased;
	float m_flProjTimeElapsed;
	float m_flProjScriptTime;
	Vector2f m_vProjOrigin;
	float m_flProjRotationBase;
	Vector2f m_vProjVelocityBase;
	DestinyFunction m_fDestinyFunction;
};