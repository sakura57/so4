/**
 * Copyright (C) 2020 Jacob Farnsworth.
 *
 * This file is part of the Spaced Out 4 project.
 *
 * Spaced Out 4 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, version 2.
 *
 * Spaced Out 4 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "Archetype.hxx"
#include "EngineProtos.hxx"

class CBase
{
public:
	struct CreationParameters
	{
		std::string szName;
		std::string szDescription;
		std::string szRmsnScript;
		std::string szType;
		FactionId iFaction;
		std::vector<ArchId> vEquipmentSelling;
		int iMetalExchange;
		SectorId iLaunchSector;
		Vector2f vLaunchPos;

		std::string szRmsnName;
		std::string szRmsnDesc;
		bool bRmsnOffered;
		int iRmsnReward;
	};

	CBase();
	CBase(CreationParameters const &);
	~CBase();

	std::string get_name(void) const;
	std::string get_description(void) const;
	std::string get_rmsn_script(void) const;
	std::string get_type(void) const;
	FactionId get_faction(void) const;
	std::vector<ArchId> get_equipment_selling(void) const;
	int get_metal_exchange(void) const;
	SectorId get_launch_sector(void) const;
	Vector2f get_launch_pos(void) const;

	std::string get_rmsn_name(void) const;
	std::string get_rmsn_desc(void) const;
	int get_rmsn_reward(void) const;
	bool get_rmsn_offered(void) const;

private:
	std::string m_szName;
	std::string m_szDescription;
	std::string m_szRmsnScript;
	std::string m_szType;
	FactionId m_iFaction;
	std::vector<ArchId> m_vEquipmentSelling;
	int m_iMetalExchange;

	SectorId m_iLaunchSector;
	Vector2f m_vLaunchPos;

	std::string m_szRmsnName;
	std::string m_szRmsnDesc;
	int m_iRmsnReward;
	bool m_bRmsnOffered;
};