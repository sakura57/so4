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

#include "ISpell.hxx"

class CSpellNanobotStream : public ISpell
{
public:
	virtual std::string get_name(void)
	{
		return "Nanobot Stream";
	};

	virtual std::string get_desc(void)
	{
		return "Restore 10% HP over 10s";
	};

	virtual StaticTextureId get_icon_texture(void)
	{
		return 15;
	};

	virtual float get_cooldown(void)
	{
		return 20.0f;
	};

	virtual float get_mana_cost(void)
	{
		return 30.0f;
	};

	virtual bool cast(InstanceId const, InstanceId const);
private:
};