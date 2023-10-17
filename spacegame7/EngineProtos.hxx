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

#include "Defs.hxx"

interface IWorldInstance;
interface IWorldObject;
interface IPhysicalObject;

interface IEngine;
interface IWorld;
interface IScriptEngine;
interface IRenderPipeline;

interface ICamera;
interface ILiving;

class SGEngine;
class CObject;
class CEquippedObject;
class CShip;
class CEquipInstance;
class CChaseCamera;

interface IListener;
interface IListenable;

typedef unsigned int SoundId;
typedef unsigned int CharacterId;
typedef unsigned int FactionId;
typedef unsigned int AnimationId;
typedef unsigned int NameCollectionId;
typedef unsigned int ArchId;
typedef unsigned int ArchType;
typedef unsigned int DropTableId;
typedef unsigned int CharacterEntityId;
typedef int StaticTextureId;
typedef int SectorId;
typedef int BaseId;
typedef unsigned int AstfieldId;