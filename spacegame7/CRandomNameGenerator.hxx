#pragma once

#include <vector>

struct CNameCollection
{
	std::vector<std::string> m_vFirstNamesMale;
	std::vector<std::string> m_vFirstNamesFemale;
	std::vector<std::string> m_vMiddleNames;
	std::vector<std::string> m_vLastNames;
};

typedef unsigned int NameCollectionId;