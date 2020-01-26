#pragma once

#include "ISaver.hxx"
#include "IMatManager.hxx"

class CMaterialsSaver : public ISaver
{
public:
	CMaterialsSaver(IMatManager*);

	virtual void dump_to_save(std::string const&);
	virtual void load_from_save(std::string const&);
private:
	IMatManager* m_pMatMgr;
};