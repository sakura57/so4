#pragma once

#include <mutex>
#include <unordered_map>
#include <deque>
#include "IMatManager.hxx"
#include "IMaterial.hxx"
#include "CAstfield.hxx"

//TODO: I'm fairly confident this class is working as intended (TM),
//but it could use a thorough testing esp to make sure it isn't leaking memory
class CMatManager : public IMatManager
{
public:
	CMatManager();
	~CMatManager();

	//members inherited from ISaveable
	virtual void dump_to_save(std::string const &) final;
	virtual void load_from_save(std::string const &) final;

	virtual void do_possible_generation_tick(float const);
	virtual void force_generation_tick(void);
	virtual void add_material_category(MaterialClassId const, IMaterialClass*);

	virtual IMaterial * pull_random_material_from_pool(MaterialType const);
	virtual IMaterial * pull_random_material_from_pool_hazy_associative(AstfieldId const, MaterialType const);

	virtual IMaterial* get_material_from_serial(std::string const&);

	virtual IMaterialClass* get_material_class_from_id(MaterialClassId const);

	virtual void add_material(std::string const&, IMaterial*);
	virtual void make_hazy_association(AstfieldId const, IMaterial*);

	virtual void get_materials(MatSerialMap&);

	virtual void get_material_associations(std::unordered_map<AstfieldId, IMaterial*>&);

	virtual void reset(void);

	virtual void build_material_pools(void);

	virtual float get_time_since_last_update(void);

	virtual void set_last_update_time(float const);

	virtual IMaterial* get_material_association(AstfieldId const);

	//virtual void get_material_pools(std::unordered_map<MaterialType, std::deque<IMaterial*>> &);

	//virtual void add_material_to_pool(IMaterial *);

	//virtual void shuffle_material_pools(void);

	//virtual void empty_material_pools(void);

public:
	static std::string generate_serial_number(void);
	static std::string generate_name(void);

private:
	void regenerate_material_pool(void);
	void clear_material_pool(void);

	/*
	 * Remove any materials that have zero references.
	 * Call occasionally but not too often, ideally
	 * during the regeneration tick.
	 */
	void cleanup_materials(void);

	/*
	 * Generate n new random resource materials and add them
	 * to the pool. Call during regen, call lazily if no more are available
	 * when system manager tries to pull.
	 */
	void generate_materials(int const);

private:
	std::mutex m_mFieldAccess;

	/*
	 * Storage for mat classes
	 */
	std::vector<IMaterialClass*> m_vMaterialClasses;

	/*
	 * the POOL of available mats handed out to asteroid fields, sorted by type
	 * NOT storage for mats in general, that is handled by m_mMatSerialMap
	 */
	std::unordered_map<MaterialType, std::deque<IMaterial*>> m_vMaterialPool;

	std::unordered_map<AstfieldId, IMaterial*> m_mAstfieldMaterialMapping;

	/*
	 * Storage for mats, mapping serial number to the mat
	 */
	MatSerialMap m_mMatSerialMap;

	float m_flLastUpdate;
};