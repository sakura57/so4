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

#include <lua.hpp>

#include "Defs.hxx"
#include "IWorldInstance.hxx"
#include "Listeners.hxx"
#include "ITransientStateStructure.hxx"

typedef unsigned int CallbackId;

/*
 * script engine interface
 */
interface IScriptEngine : public ITransientStateStructure
{
	virtual void initialize(void) = 0;

	virtual void script_tick(float const) = 0;

	//load and enqueue a script's main method for execution
	virtual void script_enqueue(char const *) = 0;

	//enqueue a particular function for execution
	virtual CallbackId script_enqueue_function(float const, char const *) = 0;

	virtual void register_callback(char const *, void *) = 0;
};