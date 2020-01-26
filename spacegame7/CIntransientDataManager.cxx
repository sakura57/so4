#include "CIntransientDataManager.hxx"
#include "CCharacterEntityManager.hxx"

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

	this->m_mStringVariables[szKey] = szValue;
}

std::string CIntransientDataManager::get_string_variable(std::string const &szKey)
{
	SCOPE_LOCK(this->m_mFieldAccess);

	return this->m_mStringVariables[szKey];
}