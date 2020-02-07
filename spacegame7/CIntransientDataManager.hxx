#pragma once

#include "ICharacterEntityManager.hxx"
#include "IIntransientDataManager.hxx"

#include <map>

class CIntransientDataManager : public IIntransientDataManager
{
public:
	CIntransientDataManager();
	~CIntransientDataManager();

	//members inherited from ISaveable
	virtual void dump_to_save(std::string const&) final;
	virtual void load_from_save(std::string const&) final;

	virtual ICharacterEntityManager *get_character_entity_manager(void);
	virtual void set_string_variable(std::string const &, std::string const &);
	virtual std::string get_string_variable(std::string const &);

	virtual std::string dump_string_variables(void) final;
	virtual void clear_string_variables(void) final;

private:
	std::mutex m_mFieldAccess;
	ICharacterEntityManager *m_pCharEntityManager;
	std::map<std::string, std::string> m_mStringVariables;
};