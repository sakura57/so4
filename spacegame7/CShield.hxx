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
#pragma once

#include "ILiving.hxx"
#include "CPhantomObject.hxx"

class CShield : public CPhantomObject, public virtual ILiving
{
public:
	enum
	{
		InstanceFlag = 1 << 15
	};

	struct CreationParameters : CPhantomObject::CreationParameters
	{
		InstanceId iParentInstanceId;
	};

	CShield(void);

	//members inherited from IWorldInstance
	virtual void instance_create(InstanceId const);

	//members inherited from IWorldObject
	virtual ~CShield(void);
	virtual Vector2f get_position(void) final;
	virtual void set_position(Vector2f const &) final;
	virtual float get_rotation(void) final;
	virtual void set_rotation(float const) final;
	virtual AppearanceId get_appearance(void) final;
	virtual void set_appearance(AppearanceId) final;

	//members inherited from ILiving
	virtual void alive_tick(float const) final;
	virtual void alive_set(bool const) final;

	virtual bool is_alive(void) final
	{
		return true;
	};

	//members defined in CShield
	virtual void initialize(CreationParameters const &);

	virtual void get_render_parms(RenderParameters &renderParms)
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		renderParms.vScale = Vector2f(1.0f, 1.0f);
		renderParms.fAlpha = 1275.0f * this->m_fBlinkRemaining;
	};

	void register_hit()
	{
		SCOPE_LOCK(this->m_mFieldAccess);

		this->m_fBlinkRemaining = 0.1f;
	};

protected:
	AppearanceId m_uiAppearanceId;

private:
	InstanceId m_iParentInstanceId;

	float m_fBlinkRemaining;

	IWorldObject *get_parent(void)
	{
		if(!SG::get_engine()->instance_is_allocated(this->m_iParentInstanceId))
		{
			return nullptr;
		}

		IWorldInstance *pInstance = SG::get_engine()->instance_get(this->m_iParentInstanceId);

		if (pInstance->instance_get_flags() & IWorldObject::InstanceFlag)
		{
			return static_cast<IWorldObject*>(pInstance);
		}
		else
		{
			return nullptr;
		}
	};
};