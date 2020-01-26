#pragma once

#include "Defs.hxx"

/*
 * Represents an object whose data can be dumped to or loaded from
 * a savegame.
 */
interface ISaveable
{
	virtual void dump_to_save(std::string const &) = 0;
	virtual void load_from_save(std::string const &) = 0;
};