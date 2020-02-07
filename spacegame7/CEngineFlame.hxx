#pragma once

#include <mutex>
#include "CPhantomObject.hxx"
#include "SGLib.hxx"
#include "CShip.hxx"
#include "CWorldTransaction.hxx"

class CEngineFlame : public CPhantomObject
{
public:
	enum
	{
		InstanceFlag = 1 << 14
	};

	struct CreationParameters : CPhantomObject::CreationParameters
	{
		AppearanceId uiAppearanceId;
		InstanceId iParentInstanceId;
	};

	CEngineFlame(void);

	//members inherited from IWorldInstance
	virtual void instance_create(InstanceId const);

	//members inherited from IWorldObject
	virtual ~CEngineFlame(void);
	virtual Vector2f get_position(void) final;
	virtual void set_position(Vector2f const &) final;
	virtual float get_rotation(void) final;
	virtual void set_rotation(float const) final;
	virtual AppearanceId get_appearance(void) final;
	virtual void set_appearance(AppearanceId) final;

	//members defined in CEngineFlame
	virtual void initialize(CreationParameters const &);

	virtual void get_render_parms(RenderParameters &renderParms)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		IWorldObject *pParent = this->get_parent();

		if(pParent != nullptr)
		{
			float fParentSpeed = pParent->get_physical()->get_acceleration().magnitude() / 10.0f;

			float fHorizScale = -0.5f / ((1.0f / 2.5f) * fParentSpeed + 1.0f) + 1.0f;
			float fVertScale = -0.75f / ((1.0f / 10.0f) * fParentSpeed + 1.0f) + 1.0f;

			float fAlpha = -127.0f / ((1.0f / 10.0f) * fParentSpeed + 1.0f) + 255.0f;

			renderParms.vScale = Vector2f(fHorizScale, fVertScale);
			renderParms.fAlpha = fAlpha;
		}
		else
		{
			renderParms.vScale = Vector2f(0.0f, 0.0f);
			renderParms.fAlpha = 0.0f;
		}
	};
protected:
	AppearanceId m_uiAppearanceId;

private:
	InstanceId m_iParentInstanceId;

	IWorldObject *get_parent(void)
	{
		if(!SG::get_engine()->instance_is_allocated(this->m_iParentInstanceId))
		{
			return nullptr;
		}

		IWorldInstance *pInstance = SG::get_engine()->instance_get(this->m_iParentInstanceId);

		if(pInstance->instance_get_flags() & IWorldObject::InstanceFlag)
		{
			return static_cast<IWorldObject*>(pInstance);
		}
		else
		{
			return nullptr;
		}
	};
};