#pragma once

#include "Defs.hxx"
#include <mutex>
#include "IMaterialBank.hxx"

class CMaterialBank : public IMaterialBank
{
public:
	CMaterialBank();
	~CMaterialBank();

	/*
	 * UNSAFE with regard to mat reference count, do NOT use unless you
	 * have otherwise controlled ref counts for mats
	 */
	virtual void get_material_quantity_map(MaterialQuantityMap &);

	/*
	 * UNSAFE with regard to mat reference count, do NOT use unless you
	 * have otherwise controlled ref counts for mats
	 */
	virtual void set_material_quantity_map(MaterialQuantityMap const &);

	/*
	 * SAFE with regard to mat reference count
	 */
	virtual unsigned int add_material(std::string const &, unsigned int const);
	virtual unsigned int get_material_quantity(std::string const &);

	/*
	 * SAFE with regard to mat reference count
	 */
	virtual void set_material_quantity(std::string const &, unsigned int const);

private:
	std::mutex m_mFieldAccess;
	MaterialQuantityMap m_mMatQuantityMap;
};