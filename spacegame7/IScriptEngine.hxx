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