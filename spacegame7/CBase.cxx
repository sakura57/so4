#include "CBase.hxx"

CBase::CBase()
{
	
}

CBase::CBase(CBase::CreationParameters const &creationParms)
{
	this->m_iFaction = creationParms.iFaction;
	this->m_iMetalExchange = creationParms.iMetalExchange;
	this->m_szDescription = creationParms.szDescription;
	this->m_szName = creationParms.szName;
	this->m_szRmsnScript = creationParms.szRmsnScript;
	this->m_szType = creationParms.szType;
	this->m_vEquipmentSelling = creationParms.vEquipmentSelling;
	this->m_iLaunchSector = creationParms.iLaunchSector;
	this->m_vLaunchPos = creationParms.vLaunchPos;
	this->m_szRmsnName = creationParms.szRmsnName;
	this->m_szRmsnDesc = creationParms.szRmsnDesc;
	this->m_bRmsnOffered = creationParms.bRmsnOffered;
	this->m_iRmsnReward = creationParms.iRmsnReward;
}

CBase::~CBase()
{
}

std::string CBase::get_name(void) const
{
	return this->m_szName;
}

std::string CBase::get_description(void) const
{
	return this->m_szDescription;
}

std::string CBase::get_rmsn_script(void) const
{
	return this->m_szRmsnScript;
}

std::string CBase::get_type(void) const
{
	return this->m_szType;
}

FactionId CBase::get_faction(void) const
{
	return this->m_iFaction;
}

std::vector<ArchId> CBase::get_equipment_selling(void) const
{
	return this->m_vEquipmentSelling;
}

int CBase::get_metal_exchange(void) const
{
	return this->m_iMetalExchange;
}

SectorId CBase::get_launch_sector(void) const
{
	return this->m_iLaunchSector;
}

Vector2f CBase::get_launch_pos(void) const
{
	return this->m_vLaunchPos;
}

int CBase::get_rmsn_reward(void) const
{
	return this->m_iRmsnReward;
}

bool CBase::get_rmsn_offered(void) const
{
	return this->m_bRmsnOffered;
}

std::string CBase::get_rmsn_name(void) const
{
	return this->m_szRmsnName;
}

std::string CBase::get_rmsn_desc(void) const
{
	return this->m_szRmsnDesc;
}