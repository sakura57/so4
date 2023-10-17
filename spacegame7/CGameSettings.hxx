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

#include <string>
#include <mutex>
#include <unordered_map>
#include "Util.hxx"

class CGameSettings
{
public:
	template<class T>
	bool get_setting(std::string const &szSetting, T &setting)
	{
		using ContainerType = std::unordered_map<std::string, T>;

		SCOPE_LOCK(CGameSettings::m_mFieldAccess);

		ContainerType &storageContainer = CGameSettings::get_storage_container<T>();
		ContainerType::iterator i = storageContainer.find(szSetting);

		if(i == storageContainer.end())
		{
			return false;
		}

		setting = i->second;
		return true;
	};

	template<class T>
	void set_setting(std::string const &szSetting, T const &newValue)
	{
		using ContainerType = std::unordered_map<std::string, T>;

		SCOPE_LOCK(CGameSettings::m_mFieldAccess);

		ContainerType &storageContainer = CGameSettings::get_storage_container<T>();

		storageContainer[szSetting] = newValue;
	};

private:
	template<class T> 
	static std::unordered_map<std::string, T> &get_storage_container(void)
	{
		static std::unordered_map<std::string, T> storageContainer;

		return storageContainer;
	};

	static std::mutex m_mFieldAccess;
};