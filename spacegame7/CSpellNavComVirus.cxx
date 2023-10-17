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
#include "CSpellNavComVirus.hxx"
#include "SGLib.hxx"
#include "CShip.hxx"
#include "CStatusEngineOffline.hxx"

bool CSpellNavComVirus::cast(InstanceId const iCaster, InstanceId const iTarget)
{
	if(iCaster == INVALID_INSTANCE || iTarget == INVALID_INSTANCE)
	{
		return false;
	}

	CShip *pCaster = SG::get_engine()->instance_get_checked<CShip>(iCaster);
	CEquippedObject *pTarget = SG::get_engine()->instance_get_checked<CEquippedObject>(iTarget);

	if(pCaster == nullptr)
	{
		return false;
	}

	if(pTarget == nullptr)
	{
		return false;
	}

	SG::get_audio_manager()->play_sound(14);
	SG::get_particle_manager()->add_particle(12, pTarget->get_position(), pTarget->get_velocity(), 0.0f, 0.0f);

	pTarget->inflict_status(new CStatusEngineOffline(5.0f));
	pTarget->perform_hostile_action(iCaster);

	return true;
}