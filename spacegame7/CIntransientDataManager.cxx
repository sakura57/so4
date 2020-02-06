#include <sstream>

#include "CIntransientDataManager.hxx"
#include "CCharacterEntityManager.hxx"
#include "CIntransientDataSaver.hxx"

CIntransientDataManager::CIntransientDataManager()
{
	this->m_pCharEntityManager = new CCharacterEntityManager;
}

CIntransientDataManager::~CIntransientDataManager()
{
	delete this->m_pCharEntityManager;
	this->m_pCharEntityManager = nullptr;
}

ICharacterEntityManager *CIntransientDataManager::get_character_entity_manager(void)
{
	return this->m_pCharEntityManager;
}

void CIntransientDataManager::set_string_variable(std::string const &szKey, std::string const &szValue)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	if(szValue.size() == 0)
	{
		throw SGException("Blank global values not permitted");
	}

	this->m_mStringVariables[szKey] = szValue;
}

std::string CIntransientDataManager::get_string_variable(std::string const &szKey)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	auto i = this->m_mStringVariables.find(szKey);

	if(i != this->m_mStringVariables.end())
	{
		return i->second;
	}
	else
	{
		return std::string("");
	}
}

void CIntransientDataManager::dump_to_save(std::string const& szSaveFile)
{
	CIntransientDataSaver(this).dump_to_save(szSaveFile);
}

void CIntransientDataManager::load_from_save(std::string const& szSaveFile)
{
	CIntransientDataSaver(this).load_from_save(szSaveFile);
}

std::string CIntransientDataManager::dump_string_variables(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	bool bFirst = true;
	std::stringstream ss;

	for(auto i = this->m_mStringVariables.begin(); i != this->m_mStringVariables.end(); ++i)
	{
		if(!bFirst)
		{
			ss << " ";

			bFirst = false;
		}

		ss << i->first;
		ss << " ";
		ss << i->second;
	}

	return ss.str();
}

void CIntransientDataManager::clear_string_variables(void)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	this->m_mStringVariables.clear();
}