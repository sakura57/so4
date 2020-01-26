#pragma once

#include "ICharacterEntityManager.hxx"
#include "IIntransientDataManager.hxx"

#include <map>

class CIntransientDataManager : public IIntransientDataManager
{
public:
	CIntransientDataManager();
	~CIntransientDataManager();

	virtual ICharacterEntityManager *get_character_entity_manager(void);
	virtual void set_string_variable(std::string const &, std::string const &);
	virtual std::string get_string_variable(std::string const &);

private:
	std::mutex m_mFieldAccess;
	ICharacterEntityManager *m_pCharEntityManager;
	std::map<std::string, std::string> m_mStringVariables;
};