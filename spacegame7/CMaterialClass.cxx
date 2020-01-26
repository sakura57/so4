#include "CMaterialClass.hxx"
#include "SGLib.hxx"

CMaterialClass::CMaterialClass(std::string const& sName, MaterialType const matType, unsigned int const weight)
	: m_sName(sName), m_matType(matType), m_uiWeight(weight)
{

}

CMaterialClass::~CMaterialClass()
{

}

std::string CMaterialClass::get_name()
{
	return this->m_sName;
}

MaterialType CMaterialClass::get_type()
{
	return this->m_matType;
}

AttributeRange CMaterialClass::get_attribute_range(MaterialAttribute const attrib)
{
	auto i = this->m_mAttribRanges.find(attrib);

	if(i != this->m_mAttribRanges.end())
	{
		return i->second;
	}
	else
	{
		throw SGException("Material does not possess attribute");
	}
}

unsigned int CMaterialClass::get_generation_weight(void)
{
	return this->m_uiWeight;
}

void CMaterialClass::set_attribute_range(MaterialAttribute const attrib, AttributeRange const &range)
{
	this->m_mAttribRanges[attrib] = range;
}

void CMaterialClass::construct_randomized_attribute_panel(std::unordered_map<MaterialAttribute, float>& attribMap)
{
	for(auto p : this->m_mAttribRanges)
	{
		std::uniform_real_distribution<float> attribDist(std::get<0>(p.second), std::get<1>(p.second));

		float flValue = attribDist(SG::get_random());

		attribMap[p.first] = flValue;
	}
}