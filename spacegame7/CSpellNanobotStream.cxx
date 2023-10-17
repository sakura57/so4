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
#include "CSpellNanobotStream.hxx"
#include "SGLib.hxx"
#include "CShip.hxx"
#include "CStatusNanobotStream.hxx"

bool CSpellNanobotStream::cast(InstanceId const iCaster, InstanceId const iTarget)
{
	if(iCaster == INVALID_INSTANCE)
	{
		return false;
	}

	CShip *pCaster = SG::get_engine()->instance_get_checked<CShip>(iCaster);

	if(pCaster == nullptr)
	{
		return false;
	}

	SG::get_audio_manager()->play_sound(16);
	//SG::get_particle_manager()->add_particle(11, pCaster->get_position(), pCaster->get_velocity(), 0.0f, 0.0f);

	pCaster->inflict_status(new CStatusNanobotStream(10.0f));

	return true;
}