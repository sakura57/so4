#pragma once

#include "Defs.hxx"

/*
 * Represents the implementation of saving/loading for a particular
 * ISaveable class.
 */
interface ISaver
{
	virtual void dump_to_save(std::string const&) = 0;
	virtual void load_from_save(std::string const&) = 0;
};