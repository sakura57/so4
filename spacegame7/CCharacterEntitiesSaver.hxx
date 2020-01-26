#pragma once

#include "ISaver.hxx"
#include "ICharacterEntityManager.hxx"

class CCharacterEntitiesSaver : public ISaver
{
public:
	CCharacterEntitiesSaver(ICharacterEntityManager*);

	virtual void dump_to_save(std::string const&);
	virtual void load_from_save(std::string const&);
private:
	ICharacterEntityManager* m_pCEM;
};