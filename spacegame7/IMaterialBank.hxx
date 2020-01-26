#pragma once

#include "Defs.hxx"
#include "IMaterialClass.hxx"
#include <unordered_map>

typedef std::unordered_map<std::string, unsigned int> MaterialQuantityMap;

interface IMaterialBank
{
	virtual void get_material_quantity_map(MaterialQuantityMap &) = 0;
	virtual void set_material_quantity_map(MaterialQuantityMap const &) = 0;
	virtual unsigned int add_material(std::string const&, unsigned int const) = 0;
	virtual unsigned int get_material_quantity(std::string const &) = 0;
	virtual void set_material_quantity(std::string const&, unsigned int const) = 0;
};