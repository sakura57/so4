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

#include "ISpellManager.hxx"

#define SPELL_NAVCOM_VIRUS 1
#define SPELL_NANOBOT_STREAM 2
#define SPELL_NANOBOT_SWARM 3
#define SPELL_MAGNETIC_DRAIN 4
#define SPELL_SIGNATURE_SPOOF 5
#define SPELL_PERFECT_DEFENSE 6
#define SPELL_KINETIC_BARRAGE_WEAK 7
#define SPELL_KINETIC_BARRAGE_STRONG 8
#define SPELL_LASER_BARRAGE_WEAK 9
#define SPELL_LASER_BARRAGE_STRONG 10
#define SPELL_MISSILE_SWARM_WEAK 11
#define SPELL_MISSILE_SWARM_STRONG 12
#define SPELL_HELLFIRE_RAIN 13
#define SPELL_MISSILE_MASSACRE 14
#define SPELL_WARPDRIVE_UPLINK 15

void add_all_spells(ISpellManager *);