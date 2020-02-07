#pragma once

#include "ISaver.hxx"
#include "IIntransientDataManager.hxx"

class CIntransientDataSaver : public ISaver
{
public:
	CIntransientDataSaver(IIntransientDataManager*);

	virtual void dump_to_save(std::string const&);
	virtual void load_from_save(std::string const&);
private:
	IIntransientDataManager* m_pIDM;
};