#pragma once

#include "Defs.hxx"
#include "MaterialType.hxx"
#include "IMaterialClass.hxx"
#include "MaterialAttribute.hxx"

interface IMaterial
{
	virtual std::string get_name(void) = 0;
	virtual std::string get_serial_number(void) = 0;
	virtual MaterialClassId get_material_class(void) = 0;

	virtual void set_attribute(MaterialAttribute const, float const) = 0;
	virtual float get_attribute(MaterialAttribute const) = 0;

	virtual void get_attribute_set(std::unordered_map<MaterialAttribute, float>&) = 0;

	/*
	 * Materials implement a reference count system.
	 * If they are currently in the mat manager's generation
	 * pool, they always have at least one reference.
	 * Otherwise, other references are probably associated
	 * with the player's inventory.
	 *
	 * Upon reaching zero references they should be removed.
	 */
	virtual unsigned int get_reference_count(void) = 0;
	virtual void increment_reference_count(void) = 0;
	virtual void decrement_reference_count(void) = 0;
};