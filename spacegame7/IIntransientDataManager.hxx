#pragma once

#include "Util.hxx"
#include "ICharacterEntityManager.hxx"
#include "ISaveable.hxx"

interface IIntransientDataManager : public ISaveable
{
	virtual ICharacterEntityManager *get_character_entity_manager(void) = 0;
	virtual void set_string_variable(std::string const &, std::string const &) = 0;
	virtual std::string get_string_variable(std::string const &) = 0;
	virtual std::string dump_string_variables(void) = 0;
	virtual void clear_string_variables(void) = 0;
};