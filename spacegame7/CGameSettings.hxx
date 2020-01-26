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