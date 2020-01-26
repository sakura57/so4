#pragma once

#include "IUniverse.hxx"
#include "CSector.hxx"

class CUniverse : public IUniverse
{
public:
	CUniverse();
	~CUniverse();

	virtual void add_sector(SectorId const, std::string const &, std::string const &, std::string const &, FactionId const) final;
	virtual void add_base(BaseId const, CBase::CreationParameters const &) final;
	virtual void add_astfield(AstfieldId const, CAstfield::CreationParameters const&) final;
	virtual CSector const &get_sector(SectorId const) const final;
	virtual CBase const &get_base(BaseId const) const final;
	virtual CAstfield const& get_astfield(AstfieldId const) const final;
private:
	std::vector<CSector> m_vSectors;
	std::vector<CBase> m_vBases;
	std::vector<CAstfield> m_vAstfields;
};