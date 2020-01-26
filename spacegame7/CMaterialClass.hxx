#pragma once

#include "IMaterialClass.hxx"
#include <unordered_map>
#include <tuple>

class CMaterialClass : public IMaterialClass
{
public:
	CMaterialClass(std::string const &, MaterialType const, unsigned int const);
	~CMaterialClass();

	virtual std::string get_name(void);
	virtual MaterialType get_type(void);
	virtual AttributeRange get_attribute_range(MaterialAttribute const);
	virtual unsigned int get_generation_weight(void);
	virtual void construct_randomized_attribute_panel(std::unordered_map<MaterialAttribute, float> &attribMap);

	void set_attribute_range(MaterialAttribute const, AttributeRange const&);

private:
	std::string m_sName;
	MaterialType m_matType;
	unsigned int m_uiWeight;
	
	std::unordered_map<MaterialAttribute, AttributeRange> m_mAttribRanges;
};