#pragma once

#include "Defs.hxx"
#include "IMaterialClass.hxx"
#include "IMaterial.hxx"
#include "EngineProtos.hxx"
#include <unordered_map>
#include <deque>
#include "ISaveable.hxx"

typedef std::unordered_map<std::string, IMaterial*> MatSerialMap;

interface IMatManager : public ISaveable
{
	/*
	 * Should be called as little as possible, preferably
	 * during state changes.
	 */
	virtual void do_possible_generation_tick(float const) = 0;

	virtual void force_generation_tick(void) = 0;

	virtual void add_material_category(MaterialClassId const, IMaterialClass*) = 0;

	/*
	 * Pull a random mat from the pool, given a certain mat type.
	 *
	 * IMPORTANT: It is VITAL to call decrement_reference_count()
	 * once you are finished using the material. Ast fields could do this
	 * in their destructor, for example.
	 */
	virtual IMaterial* pull_random_material_from_pool(MaterialType const) = 0;

	/*
	 * Pull a random mat from the pool, making an association with a given astfield
	 * id. This mat will be cached and given again if requested while the same
	 * astfield ID is given before the next mat pool regeneration tick has occurred.
	 */
	virtual IMaterial* pull_random_material_from_pool_hazy_associative(AstfieldId const, MaterialType const) = 0;

	virtual IMaterial* get_material_from_serial(std::string const&) = 0;

	virtual IMaterialClass* get_material_class_from_id(MaterialClassId const) = 0;

	virtual void add_material(std::string const&, IMaterial*) = 0;

	virtual void make_hazy_association(AstfieldId const, IMaterial*) = 0;

	virtual void get_material_associations(std::unordered_map<AstfieldId, IMaterial*>&) = 0;

	virtual IMaterial *get_material_association(AstfieldId const) = 0;

	virtual void get_materials(MatSerialMap&) = 0;

	virtual void reset(void) = 0;

	virtual void build_material_pools(void) = 0;

	virtual float get_time_since_last_update(void) = 0;

	virtual void set_last_update_time(float const) = 0;

	//virtual void get_material_pools(std::unordered_map<MaterialType, std::deque<IMaterial*>>&) = 0;

	//virtual void add_material_to_pool(IMaterial*) = 0;

	//virtual void shuffle_material_pools(void) = 0;

	//virtual void empty_material_pools(void) = 0;
};