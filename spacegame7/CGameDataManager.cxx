#include <algorithm>
#include <iostream>
#include <sstream>

#include "ini_parser.h"
#include "CGameDataManager.hxx"
#include "SGLib.hxx"
#include "CSkillDataManager.hxx"
#include "CBaseTransitionState.hxx"
#include "CMaterialClass.hxx"
#include <deque>
#include <set>
#include <cstdio>
#include <thread>

//TODO: included for filesystem functions,
//make them more portable instead
#include <Windows.h>

#define LOADER_MAX_SECTIONS_BUFFER_SIZE 8192
#define LOADER_MAX_VALUE_BUFFER_SIZE 2048

bool CGameDataManager::settingsLoaded = false;

CGameDataManager::CGameDataManager(void)
{
	if(!CGameDataManager::settingsLoaded)
	{
		this->load_settings_data();

		CGameDataManager::settingsLoaded = true;
	}

	this->m_pSkillDataManager = new CSkillDataManager;
}

CGameDataManager::~CGameDataManager(void)
{
	//free loadouts
	for(Loadout const * loadout : this->m_lLoadouts)
	{
		if(loadout != nullptr)
		{
			delete loadout;
		}
	}

	for (Faction const * faction : this->m_lFactions)
	{
		if (faction != nullptr)
		{
			delete faction;
		}
	}

	for (Character const * character : this->m_lCharacters)
	{
		if (character != nullptr)
		{
			delete character;
		}
	}

	if (this->m_pSkillDataManager != nullptr)
	{
		delete this->m_pSkillDataManager;
		this->m_pSkillDataManager = nullptr;
	}
}

/*
 * load all ship data from shiparch.ini
 */
void CGameDataManager::load_shiparch_data(void)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szValueBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char * szSection = szSectionNamesBuffer;

	if (strlen(szSection) == 0)
	{
		throw SGException("shiparch.ini doesn\'t exist, or contains no sections");
	}

	while(*szSection)
	{
		unsigned int uiValueBufferLen;

		this->m_lArch.push_back(new ShipArch());

		//we know that it's a ShipArch* so a reinterpret_cast<> is fine
		ShipArch *arch = reinterpret_cast<ShipArch*>(this->m_lArch.back());

		arch->uiArchId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "id", "0");
		arch->uiShipAppearance = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "appearance", "0");
		arch->flMass = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "mass", "100");
		arch->flRotationalInertia = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "rotational_inertia", "1000");
		arch->flMaxHitPoints = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "hit_points", "100");
		arch->flMaxShieldPoints = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "shield_points", "100");
		arch->flEngineForce = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "engine_force", "1000");
		arch->flEngineTorque = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "engine_torque", "1000");
		arch->uiShieldAppearance = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "shield_appearance", "0");
		arch->flShieldRegenRate = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "shield_regen_rate", "5");
		arch->flShieldRegenPercent = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "shield_regen_percent", "0.5");
		arch->flShieldRegenTime = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "shield_regen_time", "10");
		arch->flCapacitorEnergy = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "capacitor_energy", "100");
		arch->flCapacitorRegenRate = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "capacitor_regen_rate", "10");

		uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "name", "Default Name", szValueBuffer, uiValueBufferLen);
		arch->szShipName = std::string(szValueBuffer);

		uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "short_name", "Default", szValueBuffer, uiValueBufferLen);
		arch->szShipShortName = std::string(szValueBuffer);

		uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "description", "Default Description", szValueBuffer, uiValueBufferLen);
		arch->szShipDescription = std::string(szValueBuffer);

		uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("shiparch.ini").c_str(), szSection, "hardpoints", "", szValueBuffer, uiValueBufferLen);

		float flValue = 0.0f;
		float flValueO = 0.0f;
		int iValues = 0;
		std::vector<Vector2f> hardpoints;

		std::stringstream ss(szValueBuffer);
		while (ss >> flValue)
		{
			++iValues;

			if(iValues % 2 == 0)
			{
				hardpoints.push_back(Vector2f(flValueO, flValue));
			}
			else
			{
				flValueO = flValue;
			}
		}

		//TODO: add this to the verify
		arch->hardpoints = hardpoints;

		this->verify_shiparch(*arch);

		szSection += strlen(szSection) + 1;
	}
}

/*
* load all ship data from equiparch.ini
*/
void CGameDataManager::load_equiparch_data(void)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szValueBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char * szSection = szSectionNamesBuffer;

	if (strlen(szSection) == 0)
	{
		throw SGException("equiparch.ini doesn\'t exist, or contains no sections");
	}

	while (*szSection)
	{
		unsigned int uiValueBufferLen;

		if(strncmp(szSection, "weapon", 6) == 0)
		{
			this->m_lArch.push_back(new WeaponArch());

			//we know that it's a ShipArch* so a reinterpret_cast<> is fine
			WeaponArch *arch = reinterpret_cast<WeaponArch*>(this->m_lArch.back());

			arch->uiArchId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "id", "0");
			arch->iValue = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "cost", "10");

			arch->uiProjectileAppearance = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "appearance", "0");
			arch->flShieldDamage = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "shield_damage", "1");
			arch->flHullDamage = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "hull_damage", "1");
			arch->flMuzzleVelocity = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "muzzle_velocity", "100");
			arch->flProjectileLifetime = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "projectile_lifetime", "1");
			arch->flRefireDelay = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "refire_delay", "1");
			arch->uiFireSound = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "fire_sound", "1");
			arch->flEnergyCost = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "energy_cost", "0");

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "name", "Default Name", szValueBuffer, uiValueBufferLen);
			arch->szEquipName = std::string(szValueBuffer);

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "description", "Default Description", szValueBuffer, uiValueBufferLen);
			arch->szEquipDescription = std::string(szValueBuffer);

			this->verify_weaparch(*arch);

			szSection += strlen(szSection) + 1;
		}
		else if (strncmp(szSection, "missile", 7) == 0)
		{
			this->m_lArch.push_back(new MissileArch());

			//we know that it's a ShipArch* so a reinterpret_cast<> is fine
			MissileArch *arch = reinterpret_cast<MissileArch*>(this->m_lArch.back());

			arch->uiArchId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "id", "0");
			arch->iValue = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "cost", "10");

			arch->uiProjectileAppearance = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "appearance", "0");
			arch->flShieldDamage = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "shield_damage", "1");
			arch->flHullDamage = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "hull_damage", "1");
			arch->flMuzzleVelocity = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "muzzle_velocity", "100");
			arch->flProjectileLifetime = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "projectile_lifetime", "1");
			arch->flRefireDelay = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "refire_delay", "1");
			arch->uiFireSound = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "fire_sound", "1");
			arch->flEnergyCost = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "energy_cost", "0");

			arch->flMotorLifetime = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "motor_lifetime", "1");
			arch->flMotorDelay = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "motor_delay", "1");
			arch->uiMotorSound = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "motor_sound", "1");
			arch->flMotorAcceleration = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "motor_acceleration", "1");
			arch->uiMotorParticle = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "motor_smoke_particle", "1");
			arch->uiMunitionExplosion = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "munition_explosion", "1");

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "name", "Default Name", szValueBuffer, uiValueBufferLen);
			arch->szEquipName = std::string(szValueBuffer);

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("equiparch.ini").c_str(), szSection, "description", "Default Description", szValueBuffer, uiValueBufferLen);
			arch->szEquipDescription = std::string(szValueBuffer);

			this->verify_weaparch(*arch);

			szSection += strlen(szSection) + 1;
		}
	}
}

/*
* load all loadout data from settings.ini
*/
void CGameDataManager::load_settings_data(void)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szValueBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];
	unsigned int uiValueBufferLen;

	CGameSettings *settings = SG::get_game_settings();

	settings->set_setting<unsigned int>("quality_level", CGameDataManager::read_ini_uint(".\\settings.ini", "Game", "quality_level", "4"));
	settings->set_setting<unsigned int>("controls_style", CGameDataManager::read_ini_uint(".\\settings.ini", "Game", "controls_style", "1"));
	settings->set_setting<unsigned int>("screen_width", CGameDataManager::read_ini_uint(".\\settings.ini", "Game", "screen_width", "1280"));
	settings->set_setting<unsigned int>("screen_height", CGameDataManager::read_ini_uint(".\\settings.ini", "Game", "screen_height", "720"));
	settings->set_setting<unsigned int>("fullscreen", CGameDataManager::read_ini_uint(".\\settings.ini", "Game", "fullscreen", "0"));

	//load sound volume
	float flSoundVolume = CGameDataManager::read_ini_uint(".\\settings.ini", "Game", "sound_volume", "80.0");
	sf::Listener::setGlobalVolume(flSoundVolume);
	settings->set_setting<float>("sound_volume", flSoundVolume);

	settings->set_setting<float>("music_volume", CGameDataManager::read_ini_uint(".\\settings.ini", "Game", "music_volume", "0.0"));
	//TODO: music volume still doesn't do anything

	uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
	strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
	CGameDataManager::read_ini_string(".\\settings.ini", "Game", "data_directory", ".\\data", szValueBuffer, uiValueBufferLen);
	settings->set_setting<std::string>("data_directory", szValueBuffer);
}

/*
* save settings to the settings file
*/
void CGameDataManager::save_settings_data(void)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szValueBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	CGameSettings *settings = SG::get_game_settings();

	unsigned int ui;
	if(settings->get_setting<unsigned int>("quality_level", ui))
	{
		std::string s = Conversion::uint_to_string(ui);

		ReadWriteIniKeyValueStringA("Game", "quality_level", const_cast<char *>(s.c_str()), ".\\settings.ini", true);
	}
	if (settings->get_setting<unsigned int>("controls_style", ui))
	{
		std::string s = Conversion::uint_to_string(ui);

		ReadWriteIniKeyValueStringA("Game", "controls_style", const_cast<char *>(s.c_str()), ".\\settings.ini", true);
	}
	if (settings->get_setting<unsigned int>("screen_width", ui))
	{
		std::string s = Conversion::uint_to_string(ui);

		ReadWriteIniKeyValueStringA("Game", "screen_width", const_cast<char *>(s.c_str()), ".\\settings.ini", true);
	}
	if (settings->get_setting<unsigned int>("screen_height", ui))
	{
		std::string s = Conversion::uint_to_string(ui);

		ReadWriteIniKeyValueStringA("Game", "screen_height", const_cast<char *>(s.c_str()), ".\\settings.ini", true);
	}
	if (settings->get_setting<unsigned int>("fullscreen", ui))
	{
		std::string s = Conversion::uint_to_string(ui);

		ReadWriteIniKeyValueStringA("Game", "fullscreen", const_cast<char *>(s.c_str()), ".\\settings.ini", true);
	}

	float fl;
	if(settings->get_setting<float>("sound_volume", fl))
	{
		std::string s = Conversion::float_to_string(fl);

		ReadWriteIniKeyValueStringA("Game", "sound_volume", const_cast<char*>(s.c_str()), ".\\settings.ini", true);
	}
	if(settings->get_setting<float>("music_volume", fl))
	{
		std::string s = Conversion::float_to_string(fl);

		ReadWriteIniKeyValueStringA("Game", "music_volume", const_cast<char*>(s.c_str()), ".\\settings.ini", true);
	}
}

/*
* load all loadout data from loadouts.ini
*/
void CGameDataManager::load_loadouts_data(void)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szValueBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("loadouts.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char * szSection = szSectionNamesBuffer;

	if (strlen(szSection) == 0)
	{
		throw SGException("loadouts.ini doesn\'t exist, or contains no sections");
	}

	while (*szSection)
	{
		unsigned int uiValueBufferLen;

		if (strncmp(szSection, "loadout", 7) == 0)
		{
			unsigned int uiLoadoutId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("loadouts.ini").c_str(), szSection, "id", "0");

			if(this->m_lLoadouts.size() <= uiLoadoutId)
			{
				this->m_lLoadouts.resize(uiLoadoutId + 1, nullptr);
			}

			Loadout * loadout = new Loadout;
			ArchId archId;
			this->m_lLoadouts[uiLoadoutId] = loadout;

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("loadouts.ini").c_str(), szSection, "items", "", szValueBuffer, uiValueBufferLen);
			
			std::stringstream ss(szValueBuffer);
			while(ss >> archId)
			{
				loadout->push_back(archId);
			}

			this->m_lLoadouts[uiLoadoutId] = loadout;

			szSection += strlen(szSection) + 1;
		}
	}
}

/*
* load all loadout data from names.ini
*/
void CGameDataManager::load_namecoll_data(void)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szValueBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("names.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char * szSection = szSectionNamesBuffer;

	if (strlen(szSection) == 0)
	{
		throw SGException("names.ini doesn\'t exist, or contains no sections");
	}

	while (*szSection)
	{
		unsigned int uiValueBufferLen;

		if (strncmp(szSection, "name_group", 10) == 0)
		{
			unsigned int nameCollId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("names.ini").c_str(), szSection, "id", "0");

			if (this->m_lNameCollections.size() <= nameCollId)
			{
				this->m_lNameCollections.resize(nameCollId + 1, nullptr);
			}
			
			CNameCollection * pNameCollection = new CNameCollection;
			this->m_lNameCollections[nameCollId] = pNameCollection;

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("names.ini").c_str(), szSection, "first_names_male", "", szValueBuffer, uiValueBufferLen);

			std::stringstream ss(szValueBuffer);
			std::string name;
			while (ss >> name)
			{
				if (!name.size()) continue;
				pNameCollection->m_vFirstNamesMale.push_back(name);
			}

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("names.ini").c_str(), szSection, "first_names_female", "", szValueBuffer, uiValueBufferLen);

			ss.clear();
			ss.str(szValueBuffer);
			while (ss >> name)
			{
				if (!name.size()) continue;
				pNameCollection->m_vFirstNamesFemale.push_back(name);
			}

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("names.ini").c_str(), szSection, "middle_names", "", szValueBuffer, uiValueBufferLen);

			ss.clear();
			ss.str(szValueBuffer);
			while (ss >> name)
			{
				if (!name.size()) continue;
				pNameCollection->m_vMiddleNames.push_back(name);
			}

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("names.ini").c_str(), szSection, "last_names", "", szValueBuffer, uiValueBufferLen);

			ss.clear();
			ss.str(szValueBuffer);
			while (ss >> name)
			{
				if (!name.size()) continue;
				pNameCollection->m_vLastNames.push_back(name);
			}

			this->m_lNameCollections[nameCollId] = pNameCollection;

			szSection += strlen(szSection) + 1;
		}
	}
}

void CGameDataManager::load_factions_data(void)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szValueBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("factions.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char * szSection = szSectionNamesBuffer;

	if (strlen(szSection) == 0)
	{
		throw SGException("factions.ini doesn\'t exist, or contains no sections");
	}

	while (*szSection)
	{
		unsigned int uiValueBufferLen;

		if (strncmp(szSection, "faction", 7) == 0)
		{
			unsigned int uiFactionId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("factions.ini").c_str(), szSection, "id", "0");

			if (this->m_lFactions.size() <= uiFactionId)
			{
				this->m_lFactions.resize(uiFactionId + 1, nullptr);
			}

			Faction * faction = new Faction;

			this->m_lFactions[uiFactionId] = faction;

			faction->id = uiFactionId;

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("factions.ini").c_str(), szSection, "name", "", szValueBuffer, uiValueBufferLen);
			faction->name = std::string(szValueBuffer);

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("factions.ini").c_str(), szSection, "short_name", "", szValueBuffer, uiValueBufferLen);
			faction->short_name = std::string(szValueBuffer);

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("factions.ini").c_str(), szSection, "description", "", szValueBuffer, uiValueBufferLen);
			faction->description = std::string(szValueBuffer);

			szSection += strlen(szSection) + 1;
		}
	}
}

/*
* load all base data from universe.ini
*/
void CGameDataManager::load_all_bases(IUniverse *pUniverse)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szStringBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("bases.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char * szSection = szSectionNamesBuffer;

	if(strlen(szSection) == 0)
	{
		throw SGException("bases.ini doesn\'t exist, or contains no sections");
	}

	while(*szSection)
	{
		unsigned int uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		BaseId baseId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("bases.ini").c_str(), szSection, "id", "0");
		CBase::CreationParameters baseParms;

		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("bases.ini").c_str(), szSection, "name", "", szStringBuffer, uiStringLen);
		baseParms.szName = std::string(szStringBuffer);
		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("bases.ini").c_str(), szSection, "description", "", szStringBuffer, uiStringLen);
		baseParms.szDescription = std::string(szStringBuffer);
		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("bases.ini").c_str(), szSection, "mission", "", szStringBuffer, uiStringLen);
		baseParms.szRmsnScript = std::string(szStringBuffer);
		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("bases.ini").c_str(), szSection, "mission_title", "", szStringBuffer, uiStringLen);
		baseParms.szRmsnName = std::string(szStringBuffer);
		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("bases.ini").c_str(), szSection, "mission_description", "", szStringBuffer, uiStringLen);
		baseParms.szRmsnDesc = std::string(szStringBuffer);
		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		baseParms.bRmsnOffered = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("bases.ini").c_str(), szSection, "mission_offered", "0") == 1;
		baseParms.iRmsnReward = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("bases.ini").c_str(), szSection, "mission_reward", "100");

		baseParms.iFaction = (FactionId)CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("bases.ini").c_str(), szSection, "faction", "0");
		baseParms.iMetalExchange = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("bases.ini").c_str(), szSection, "metal_exchange_rate", "0");

		baseParms.iLaunchSector = (SectorId)CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("bases.ini").c_str(), szSection, "launch_sector", "0");
		baseParms.vLaunchPos = CGameDataManager::read_ini_vector(CGameDataManager::get_full_data_file_path("bases.ini").c_str(), szSection, "launch_pos", "0, 0");

		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("bases.ini").c_str(), szSection, "equipment_selling", "", szStringBuffer, uiStringLen);
		
		ArchId archId;
		std::stringstream ss(szStringBuffer);
		while(ss >> archId)
		{
			baseParms.vEquipmentSelling.push_back(archId);
		}

		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		pUniverse->add_base(baseId, baseParms);

		szSection += strlen(szSection) + 1;
	}
}

/*
* load all sector data from universe.ini
*/
void CGameDataManager::load_all_sectors(IUniverse *pUniverse)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szStringBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("universe.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char * szSection = szSectionNamesBuffer;

	if(strlen(szSection) == 0)
	{
		throw SGException("universe.ini doesn\'t exist, or contains no sections");
	}

	while(*szSection)
	{
		unsigned int uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		SectorId sectorId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("universe.ini").c_str(), szSection, "id", "0");

		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("universe.ini").c_str(), szSection, "name", "", szStringBuffer, uiStringLen);
		std::string szSectorName(szStringBuffer);
		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
		
		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("universe.ini").c_str(), szSection, "description", "", szStringBuffer, uiStringLen);
		std::string szSectorDesc(szStringBuffer);
		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("universe.ini").c_str(), szSection, "script", "", szStringBuffer, uiStringLen);
		std::string szScriptPath(szStringBuffer);
		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		FactionId iRulingFaction = (FactionId)CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("universe.ini").c_str(), szSection, "ruling_faction", "0");

		pUniverse->add_sector(sectorId, szSectorName, szSectorDesc, szScriptPath, iRulingFaction);

		szSection += strlen(szSection) + 1;
	}
}

/*
* load all sector data from loot_tables.ini
*/
void CGameDataManager::load_all_loottables(ILootTableManager *pLootTableManager)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szStringBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("loot_tables.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char * szSection = szSectionNamesBuffer;

	if(strlen(szSection) == 0)
	{
		throw SGException("loot_tables.ini doesn\'t exist, or contains no sections");
	}

	while(*szSection)
	{
		unsigned int uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		DropTableId dropTableId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("loot_tables.ini").c_str(), szSection, "id", "0");

		pLootTableManager->create_drop_table(dropTableId);

		unsigned int uiMetalMin, uiMetalMax;

		uiMetalMin = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("loot_tables.ini").c_str(), szSection, "metal_min", "0");
		uiMetalMax = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("loot_tables.ini").c_str(), szSection, "metal_max", "0");

		pLootTableManager->set_metal_chance(dropTableId, uiMetalMin, uiMetalMax);
		
		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("loot_tables.ini").c_str(), szSection, "items", "", szStringBuffer, uiStringLen);

		ArchId uiArch = 0U;
		float flChance = 0.0f;

		std::stringstream ss(szStringBuffer);
		while(ss >> uiArch && ss >> flChance)
		{
			pLootTableManager->add_drop(dropTableId, uiArch, flChance);
		}

		szSection += strlen(szSection) + 1;
	}
}

void CGameDataManager::load_characters_data(void)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szValueBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("characters.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char * szSection = szSectionNamesBuffer;

	if (strlen(szSection) == 0)
	{
		throw SGException("characters.ini doesn\'t exist, or contains no sections");
	}

	while (*szSection)
	{
		unsigned int uiValueBufferLen;

		if (strncmp(szSection, "character", 9) == 0)
		{
			unsigned int uiCharacterId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("characters.ini").c_str(), szSection, "id", "0");

			if (this->m_lCharacters.size() <= uiCharacterId)
			{
				this->m_lCharacters.resize(uiCharacterId + 1, nullptr);
			}

			Character * character = new Character;

			this->m_lCharacters[uiCharacterId] = character;

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("characters.ini").c_str(), szSection, "name", "", szValueBuffer, uiValueBufferLen);

			std::stringstream ss(szValueBuffer);
			unsigned int nameGroup;
			ss >> nameGroup;

			if(ss.fail())
			{
				character->name = std::string(szValueBuffer);
				character->nameColl = 0;
			}
			else
			{
				character->name = std::string();
				character->nameColl = nameGroup;
			}

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("characters.ini").c_str(), szSection, "class", "Ship", szValueBuffer, uiValueBufferLen);

			character->class_type = std::string(szValueBuffer);

			character->shipArchId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("characters.ini").c_str(), szSection, "ship", "1");
			character->factionId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("characters.ini").c_str(), szSection, "faction", "1");
			character->loadoutId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("characters.ini").c_str(), szSection, "loadout", "1");
			character->pilotId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("characters.ini").c_str(), szSection, "pilot", "1");
			character->keyCharacter = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("characters.ini").c_str(), szSection, "key_character", "0");
			character->dropTableId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("characters.ini").c_str(), szSection, "drop_table", "1");
			character->ulExpValue = (unsigned long)CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("characters.ini").c_str(), szSection, "exp_value", "0");
			character->entityId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("characters.ini").c_str(), szSection, "entity_id", "0");

			szSection += strlen(szSection) + 1;
		}
	}
}

void CGameDataManager::load_pilots_data(void)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szValueBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("pilots.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char * szSection = szSectionNamesBuffer;

	if (strlen(szSection) == 0)
	{
		throw SGException("pilots.ini doesn\'t exist, or contains no sections");
	}

	while (*szSection)
	{
		unsigned int uiValueBufferLen;

		if (strncmp(szSection, "pilot", 5) == 0)
		{
			unsigned int uiPilotId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("pilots.ini").c_str(), szSection, "id", "0");

			if (this->m_lPilots.size() <= uiPilotId)
			{
				this->m_lPilots.resize(uiPilotId + 1, nullptr);
			}

			Pilot * pilot = new Pilot;

			this->m_lPilots[uiPilotId] = pilot;

			//uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			//strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			//CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("characters.ini").c_str(), szSection, "name", "", szValueBuffer, uiValueBufferLen);

			//character->name = std::string(szValueBuffer);

			pilot->flFiringCone = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("pilots.ini").c_str(), szSection, "firing_cone", "20");
			pilot->flApproachCone = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("pilots.ini").c_str(), szSection, "approach_cone", "60");
			pilot->flFiringBurstDuration = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("pilots.ini").c_str(), szSection, "firing_burst_duration", "2.0");
			pilot->flFiringBurstDelay = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("pilots.ini").c_str(), szSection, "firing_burst_delay", "0.5");
			pilot->flPreferredEnemyDistance = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("pilots.ini").c_str(), szSection, "preferred_enemy_distance", "250");
			pilot->flTargetReacquisitionTime = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("pilots.ini").c_str(), szSection, "target_reacquisition_time", "5");
			pilot->flTargetReacquisitionTime = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("pilots.ini").c_str(), szSection, "target_reacquisition_time", "5");
			pilot->flBreakoffDuration = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("pilots.ini").c_str(), szSection, "breakoff_duration", "5");
			pilot->flBreakoffDelay = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("pilots.ini").c_str(), szSection, "breakoff_delay", "5");

			szSection += strlen(szSection) + 1;
		}
	}
}

/*
 * load all graphical assets from graphics.ini
 */
void CGameDataManager::load_all_graphics_assets(IRenderPipeline *pRenderPipeline)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szImageFileBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	std::string szGraphicsDataFile = CGameDataManager::get_full_data_file_path("graphics.ini");

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, szGraphicsDataFile.c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char * szSection = szSectionNamesBuffer;

	if(strlen(szSection) == 0)
	{
		throw SGException("graphics.ini doesn\'t exist, or contains no sections");
	}

	while(*szSection)
	{
		IRenderPipeline::ImageOptions imageOpts;
		unsigned int uiImageFileLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szImageFileBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		imageOpts.uiAppearanceId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("graphics.ini").c_str(), szSection, "id", "0");
		imageOpts.vCenter = CGameDataManager::read_ini_vector(CGameDataManager::get_full_data_file_path("graphics.ini").c_str(), szSection, "center", "0, 0");
		imageOpts.vScale = CGameDataManager::read_ini_vector(CGameDataManager::get_full_data_file_path("graphics.ini").c_str(), szSection, "scale", "1, 1");
		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("graphics.ini").c_str(), szSection, "file", "", szImageFileBuffer, uiImageFileLen);
		imageOpts.bAdditive = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("graphics.ini").c_str(), szSection, "additive", "0") != 0;

		pRenderPipeline->load_image(imageOpts, szImageFileBuffer);

		szSection += strlen(szSection) + 1;
	}
}

/*
* load all graphical assets from staticobjs.ini
*/
void CGameDataManager::load_all_static_graphics_assets(IStaticTextureManager *pStaticTextureManager)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szImageFileBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("staticobjs.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char * szSection = szSectionNamesBuffer;

	if(strlen(szSection) == 0)
	{
		throw SGException("staticobjs.ini doesn\'t exist, or contains no sections");
	}

	while(*szSection)
	{
		unsigned int uiImageFileLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szImageFileBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		StaticTextureId staticTexId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("staticobjs.ini").c_str(), szSection, "id", "0");
		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("staticobjs.ini").c_str(), szSection, "file", "", szImageFileBuffer, uiImageFileLen);

		pStaticTextureManager->load_texture(staticTexId, szImageFileBuffer);

		szSection += strlen(szSection) + 1;
	}
}

/*
* load all audio assets from audio.ini
*/
void CGameDataManager::load_all_audio_assets(CAudioManager *pAudioManager)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szAudioFileBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("audio.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char * szSection = szSectionNamesBuffer;

	if (strlen(szSection) == 0)
	{
		throw SGException("audio.ini doesn\'t exist, or contains no sections");
	}

	while (*szSection)
	{
		unsigned int uiAudioFileLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szAudioFileBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		SoundId uiSoundId;

		uiSoundId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("audio.ini").c_str(), szSection, "id", "0");
		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("audio.ini").c_str(), szSection, "file", "", szAudioFileBuffer, uiAudioFileLen);
		float fVolume = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("audio.ini").c_str(), szSection, "volume", "1");

		pAudioManager->load_sound_from_file(uiSoundId, szAudioFileBuffer, fVolume);

		szSection += strlen(szSection) + 1;
	}
}

/*
* load all audio assets from animations.ini
*/
void CGameDataManager::load_all_animation_assets(CParticleManager *pParticleManager)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szAnimFileBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("animations.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char * szSection = szSectionNamesBuffer;

	if (strlen(szSection) == 0)
	{
		throw SGException("animations.ini doesn\'t exist, or contains no sections");
	}

	while (*szSection)
	{
		unsigned int uiAnimFileLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szAnimFileBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		Animation * anim = new Animation;

		anim->id = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("animations.ini").c_str(), szSection, "id", "0");
		anim->frameSize = CGameDataManager::read_ini_vector(CGameDataManager::get_full_data_file_path("animations.ini").c_str(), szSection, "size", "0, 0");
		anim->origin = CGameDataManager::read_ini_vector(CGameDataManager::get_full_data_file_path("animations.ini").c_str(), szSection, "center", "0, 0");
		anim->frames = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("animations.ini").c_str(), szSection, "frames", "0");
		anim->duration = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("animations.ini").c_str(), szSection, "duration", "0");
		anim->scale = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("animations.ini").c_str(), szSection, "scale", "0");
		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("animations.ini").c_str(), szSection, "file", "", szAnimFileBuffer, uiAnimFileLen);
		anim->alpha = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("animations.ini").c_str(), szSection, "alpha", "255");
		anim->additive = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("animations.ini").c_str(), szSection, "additive", "0");

		anim->sfTexture.loadFromFile(CGameDataManager::get_full_data_file_path(szAnimFileBuffer));

		pParticleManager->add_animation(anim->id, anim);

		szSection += strlen(szSection) + 1;
	}
}

std::string CGameDataManager::get_random_name(NameCollectionId const uiNameCollectionId)
{
	if (uiNameCollectionId > m_lNameCollections.size())
	{
		return "Invalid NCollId";
	}

	CNameCollection const * pNameCollection = this->get_name_collection(uiNameCollectionId);

	std::mt19937 &random = SG::get_random();

	unsigned int uiMale; //about 1/3 female, 2/3 male

	if (!pNameCollection->m_vFirstNamesFemale.size()) uiMale = 1;
	else uiMale = random() % 3;

	std::string szName;

	if (uiMale)
	{
		szName = pNameCollection->m_vFirstNamesMale[random() % pNameCollection->m_vFirstNamesMale.size()];
	}
	else
	{
		szName = pNameCollection->m_vFirstNamesFemale[random() % pNameCollection->m_vFirstNamesFemale.size()];
	}

	if (pNameCollection->m_vMiddleNames.size())
	{
		szName += " " + pNameCollection->m_vMiddleNames[random() % pNameCollection->m_vMiddleNames.size()];
	}

	if (pNameCollection->m_vLastNames.size())
	{
		szName += " " + pNameCollection->m_vLastNames[random() % pNameCollection->m_vLastNames.size()];
	}

	return szName;
}

/*
* szIniFile - name of the ini file to read
* szSection - section name
* szKey - key name
* szDefault - default value which will be returned if the value cannot be found
*
* returns the parsed Vector2f
*
* more or less forwards to ReadWriteIniKeyValueStringA, providing some additional
* parsing
*/
Vector2f CGameDataManager::read_ini_vector(char const *szIniFile, char const *szSection, char const *szKey, char const *szDefault)
{
	char szValueBuffer[LOADER_MAX_VALUE_BUFFER_SIZE] = "";
	unsigned int uiValueLen = LOADER_MAX_VALUE_BUFFER_SIZE;

	ReadWriteIniKeyValueStringA(szSection, szKey, szDefault, szIniFile, false, szValueBuffer, &uiValueLen, true, true);

	std::replace(szValueBuffer, szValueBuffer + strlen(szValueBuffer), ',', ' ');

	std::stringstream ss;
	ss << szValueBuffer;
	Vector2f vVector;
	ss >> vVector.x >> vVector.y;
	return vVector;
}

/*
* szIniFile - name of the ini file to read
* szSection - section name
* szKey - key name
* szDefault - default value which will be returned if the value cannot be found
* 
* returns the parsed unsigned int
*
* more or less forwards to ReadWriteIniKeyValueStringA, providing some additional
* parsing
*/
unsigned int CGameDataManager::read_ini_uint(char const *szIniFile, char const *szSection, char const *szKey, char const *szDefault)
{
	char szValueBuffer[LOADER_MAX_VALUE_BUFFER_SIZE] = "";
	unsigned int uiValueLen = LOADER_MAX_VALUE_BUFFER_SIZE;

	ReadWriteIniKeyValueStringA(szSection, szKey, szDefault, szIniFile, false, szValueBuffer, &uiValueLen, true, true);

	std::stringstream ss;
	ss << szValueBuffer;
	unsigned int uiValue;
	ss >> uiValue;
	return uiValue;
}

/*
* szIniFile - name of the ini file to read
* szSection - section name
* szKey - key name
* szDefault - default value which will be returned if the value cannot be found
*
* returns the parsed float
*
* more or less forwards to ReadWriteIniKeyValueStringA, providing some additional
* parsing
*/
float CGameDataManager::read_ini_float(char const *szIniFile, char const *szSection, char const *szKey, char const *szDefault)
{
	char szValueBuffer[LOADER_MAX_VALUE_BUFFER_SIZE] = "";
	unsigned int uiValueLen = LOADER_MAX_VALUE_BUFFER_SIZE;

	ReadWriteIniKeyValueStringA(szSection, szKey, szDefault, szIniFile, false, szValueBuffer, &uiValueLen, true, true);

	std::stringstream ss;
	ss << szValueBuffer;
	float flValue;
	ss >> flValue;
	return flValue;
}

/*
 * szIniFile - name of the ini file to read
 * szSection - section name
 * szKey - key name
 * szDefault - default value which will be returned if the value cannot be found
 * szBuffer - pointer to a buffer to receive the string
 * uiBufferLen - in: size of the provided buffer, out: number of read characters
 * 
 * more or less forwards to ReadWriteIniKeyValueStringA
 */
void CGameDataManager::read_ini_string(char const *szIniFile, char const *szSection, char const *szKey, char const *szDefault, char *szBuffer, unsigned int &uiBufferLen)
{
	ReadWriteIniKeyValueStringA(szSection, szKey, szDefault, szIniFile, false, szBuffer, &uiBufferLen, true, true);
}

void CGameDataManager::verify_shiparch(ShipArch const &arch)
{
	/*
	 * shiparch with id of 0 are not permitted
	 */
	if (arch.uiArchId == ARCH_NULL)
	{
		std::stringstream ss;
		ss << "Parsing shiparch.ini: " << "Shiparch may not have reserved id #" << arch.uiArchId;

		throw SGException(ss.str());
	}

	/*
	 * ensure we are not adding a duplicate shiparch
	 */
	unsigned int uiSameArchCount = 0;

	for(Archetype *archWalked : this->m_lArch)
	{
		if(archWalked->uiArchId == arch.uiArchId)
		{
			++uiSameArchCount;
		}
	}

	if(uiSameArchCount != 1)
	{
		std::stringstream ss;
		ss << "Parsing shiparch.ini: " << "Duplicate shiparch #" << arch.uiArchId;

		throw SGException(ss.str());
	}

	/*
	 * ships are not allowed to have appearance id 0
	 * (reserved appearance)
	 */
	if(arch.uiShipAppearance == APPEARANCE_NONE)
	{
		std::stringstream ss;
		ss << "Parsing shiparch.ini: " << "Shiparch #" << arch.uiArchId << " has reserved appearance " << arch.uiShipAppearance;

		throw SGException(ss.str());
	}
}

void CGameDataManager::verify_weaparch(WeaponArch const &arch)
{
	/*
	* shiparch with id of 0 are not permitted
	*/
	if (arch.uiArchId == ARCH_NULL)
	{
		std::stringstream ss;
		ss << "Parsing equiparch.ini: " << "Equiparch may not have reserved id #" << arch.uiArchId;

		throw SGException(ss.str());
	}

	/*
	* ensure we are not adding a duplicate shiparch
	*/
	unsigned int uiSameArchCount = 0;

	for (Archetype *archWalked : this->m_lArch)
	{
		if (archWalked->uiArchId == arch.uiArchId)
		{
			++uiSameArchCount;
		}
	}

	if (uiSameArchCount != 1)
	{
		std::stringstream ss;
		ss << "Parsing equiparch.ini: " << "Duplicate equiparch #" << arch.uiArchId;

		throw SGException(ss.str());
	}

	/*
	* ships are not allowed to have appearance id 0
	* (reserved appearance)
	*/
	if (arch.uiProjectileAppearance == APPEARANCE_NONE)
	{
		std::stringstream ss;
		ss << "Parsing equiparch.ini: " << "Shiparch #" << arch.uiArchId << " has reserved appearance " << arch.uiProjectileAppearance;

		throw SGException(ss.str());
	}
}

CSkillDataManager * CGameDataManager::get_skill_data_manager(void)
{
	return this->m_pSkillDataManager;
}

/*
 * load all ship data from shiparch.ini
 */
void CGameDataManager::load_skill_data(void)
{
	CSkillDataManager* pSDM = this->m_pSkillDataManager;

	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szValueBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("skilltrees.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char* szSection = szSectionNamesBuffer;

	if (strlen(szSection) == 0)
	{
		throw SGException("skilltrees.ini doesn\'t exist, or contains no sections");
	}

	while (*szSection)
	{
		unsigned int uiValueBufferLen;

		if (strnicmp(szSection, "skill_tree", 10) == 0)
		{
			std::string name;
			std::string desc;
			std::vector<SkillLineId> vSkillLines;
			SkillId uiFinalSkill;
			SkillTreeId uiSkillTreeId;

			uiSkillTreeId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("skilltrees.ini").c_str(), szSection, "id", "0");
			uiFinalSkill = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("skilltrees.ini").c_str(), szSection, "final_skill", "0");

			//std::cout << "Section " << szSection << " with skill tree id " << uiSkillTreeId << std::endl;

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("skilltrees.ini").c_str(), szSection, "name", "Default Name", szValueBuffer, uiValueBufferLen);
			name = std::string(szValueBuffer);

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("skilltrees.ini").c_str(), szSection, "description", "Default Description", szValueBuffer, uiValueBufferLen);
			desc = std::string(szValueBuffer);

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("skilltrees.ini").c_str(), szSection, "skill_lines", "", szValueBuffer, uiValueBufferLen);

			SkillLineId skillLineId;

			std::stringstream ss(szValueBuffer);
			while (ss >> skillLineId)
			{
				vSkillLines.push_back(skillLineId);
			}

			CSkillTree* pSkillTree = new CSkillTree(name, desc, vSkillLines, uiFinalSkill);

			pSDM->add_skill_tree(uiSkillTreeId, pSkillTree);
		}
		else if (strnicmp(szSection, "skill_line", 10) == 0)
		{
			std::string name;
			std::string desc;
			std::vector<SkillId> vSkills;
			SkillLineId uiSkillLineId;

			uiSkillLineId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("skilltrees.ini").c_str(), szSection, "id", "0");

			//std::cout << "Section " << szSection << " with skill line id " << uiSkillLineId << std::endl;

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("skilltrees.ini").c_str(), szSection, "name", "Default Name", szValueBuffer, uiValueBufferLen);
			name = std::string(szValueBuffer);

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("skilltrees.ini").c_str(), szSection, "description", "Default Description", szValueBuffer, uiValueBufferLen);
			desc = std::string(szValueBuffer);

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("skilltrees.ini").c_str(), szSection, "skills", "", szValueBuffer, uiValueBufferLen);

			SkillId skillId;

			std::stringstream ss(szValueBuffer);
			while (ss >> skillId)
			{
				vSkills.push_back(skillId);
			}

			CSkillLine* pSkillLine = new CSkillLine(name, desc, vSkills);

			pSDM->add_skill_line(uiSkillLineId, pSkillLine);
		}
		else if (strnicmp(szSection, "skill", 5) == 0)
		{
			std::string name;
			CStatBlock statBlock;
			std::vector<SpellId> vSpells;
			SkillId skillId = 0U;
			StaticTextureId iconId = 0U;

			skillId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("skilltrees.ini").c_str(), szSection, "id", "0");

			iconId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("skilltrees.ini").c_str(), szSection, "icon", "14");

			//std::cout << "Section " << szSection << " with skill id " << skillId << std::endl;

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("skilltrees.ini").c_str(), szSection, "name", "Default Name", szValueBuffer, uiValueBufferLen);
			name = std::string(szValueBuffer);

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("skilltrees.ini").c_str(), szSection, "stats", "", szValueBuffer, uiValueBufferLen);

			std::string statName;
			float flValue;

			std::stringstream ss(szValueBuffer);
			while (ss >> statName, ss >> flValue)
			{
				//std::cout << "\tGrants " << flValue << " to " << statName << std::endl;
				statBlock.set_stat(CStatBlock::get_stat_from_name(statName), flValue);
			}

			uiValueBufferLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szValueBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("skilltrees.ini").c_str(), szSection, "abilities", "", szValueBuffer, uiValueBufferLen);

			SpellId spellGranted;

			ss.clear();
			ss.str(szValueBuffer);
			while (ss >> spellGranted)
			{
				//std::cout << "\tGrants spell " << spellGranted << std::endl;
				vSpells.push_back(spellGranted);
			}
			
			CSkill* pSkill = new CSkill(skillId, name, statBlock, vSpells, iconId);

			pSDM->add_skill(skillId, pSkill);
		}
		szSection += strlen(szSection) + 1;
	}
}

void CGameDataManager::dump_to_save(std::string const& saveFile, BaseId const baseId)
{
	SG::get_game_state_manager()->notify_initiate_loading();

	std::thread savingThread(&CGameDataManager::dump_to_save_delegate, saveFile, baseId);

	savingThread.detach();
}

void CGameDataManager::dump_to_save_delegate(std::string const& saveFile, BaseId const baseId)
{
	std::vector<ISaveable*> vSaveableStructures = {
		SG::get_intransient_data_manager()->get_character_entity_manager(),
		SG::get_intransient_data_manager(),
		SG::get_material_manager()
	};

	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szStringBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];
	char const* szSaveFile = saveFile.c_str();

	bool bFoundHeaderSection = false;
	bool bFirst = true;

	CGameDataManager::rough_touch_file(saveFile);

	std::string s = Conversion::uint_to_string(baseId);
	std::stringstream ss;

	ReadWriteIniKeyValueStringA("Game", "base", const_cast<char*>(s.c_str()), saveFile.c_str(), true);

	for(ISaveable* pSaveable : vSaveableStructures)
	{
		pSaveable->dump_to_save(saveFile);
	}

	SG::get_game_state_manager()->notify_loading_finished();
}

std::future<bool> CGameDataManager::load_from_save(std::string const& saveFile)
{
	ITransientStateStructure* transientStates[] =
	{
		SG::get_script_engine(),
		SG::get_world(),
		SG::get_particle_manager()
	};

	//send a termination signal to the active game state
	SG::get_game_state_manager()->get_game_state()->state_terminating();

	SG::get_game_state_manager()->notify_initiate_loading();

	//call shifting_out() for transient state structures that might be
	//disrupted by the load
	for(ITransientStateStructure* transientState : transientStates)
	{
		transientState->shifting_out();
	}

	std::future<bool> loadingFuture = std::async(&CGameDataManager::load_from_save_delegate, saveFile);

	return loadingFuture;
}

bool CGameDataManager::load_from_save_delegate(std::string const& saveFile)
{
	std::vector<ISaveable*> vSaveableStructures = {
		SG::get_material_manager(),
		SG::get_intransient_data_manager(),
		SG::get_intransient_data_manager()->get_character_entity_manager()
	};

	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szStringBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];
	char const* szSaveFile = saveFile.c_str();

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, szSaveFile, false, szSectionNamesBuffer, &uiSectionNamesLen);

	char* szSection = szSectionNamesBuffer;

	if(strlen(szSection) == 0)
	{
		//send a loading finished notification so we don't
		//hang on a loading screen
		SG::get_game_state_manager()->notify_loading_finished();

		return false;
	}

	bool bFoundHeaderSection = false;
	BaseId baseId = 1U;

	while(*szSection)
	{
		unsigned int uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		if(!stricmp(szSection, "game"))
		{
			baseId = CGameDataManager::read_ini_uint(szSaveFile, szSection, "base", "1");

			bFoundHeaderSection = true;

			/*CGameDataManager::read_ini_string(szSaveFile, szSection, "name", "", szStringBuffer, uiStringLen);
			playerName = std::string(szStringBuffer);
			uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
			*/
		}

		szSection += strlen(szSection) + 1;
	}

	if(!bFoundHeaderSection)
	{
		//send a loading finished notification so we don't
		//hang on a loading screen
		SG::get_game_state_manager()->notify_loading_finished();

		return false;
	}

	szSection = szSectionNamesBuffer;

	for(ISaveable* pSaveable : vSaveableStructures)
	{
		pSaveable->load_from_save(saveFile);
	}

	//force a mat pool regeneration
	SG::get_material_manager()->build_material_pools();

	SG::get_game_state_manager()->transition_game_state(new CBaseTransitionState(baseId));

	SG::get_game_state_manager()->notify_loading_finished();

	return true;
}

/*
* load all resource class data from mat_categories.ini
*/
void CGameDataManager::load_all_resource_classes(IMatManager* pMatManager)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szStringBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("mat_categories.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char* szSection = szSectionNamesBuffer;

	if(strlen(szSection) == 0)
	{
		throw SGException("mat_categories.ini doesn\'t exist, or contains no sections");
	}

	while(*szSection)
	{
		unsigned int uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		MaterialClassId matClassId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("mat_categories.ini").c_str(), szSection, "id", "0");

		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("mat_categories.ini").c_str(), szSection, "name", "", szStringBuffer, uiStringLen);
		std::string szMatClassName(szStringBuffer);

		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("mat_categories.ini").c_str(), szSection, "type", "", szStringBuffer, uiStringLen);
		std::string szMatClassType(szStringBuffer);

		MaterialType matType = IMaterialClass::get_material_type_from_nick(szMatClassType);

		if(matType == MaterialType::INVALID)
		{
			throw SGException("Invalid material type");
		}

		unsigned int uiWeight = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("mat_categories.ini").c_str(), szSection, "asteroid_distribution_weight", "0");

		CMaterialClass* pMatClass = new CMaterialClass(szMatClassName, matType, uiWeight);

		std::string attribName;
		float flMin = 0.0f;
		float flMax = 0.0f;

		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("mat_categories.ini").c_str(), szSection, "attribute_ranges", "", szStringBuffer, uiStringLen);
		std::stringstream ss(szStringBuffer);
		while(ss >> attribName && ss >> flMin && ss >> flMax)
		{
			MaterialAttribute matAttrib = IMaterialClass::get_material_attrib_from_nick(attribName);

			if(matAttrib == MaterialAttribute::INVALID)
			{
				throw SGException("Invalid mat attribute");
			}

			pMatClass->set_attribute_range(matAttrib, std::make_tuple<>(flMin, flMax));
		}

		pMatManager->add_material_category(matClassId, pMatClass);

		szSection += strlen(szSection) + 1;
	}
}

/*
* load all astfield data from astfields.ini
*/
void CGameDataManager::load_all_astfields(IUniverse* pUniverse)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szStringBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("astfields.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char* szSection = szSectionNamesBuffer;

	if(strlen(szSection) == 0)
	{
		throw SGException("astfields.ini doesn\'t exist, or contains no sections");
	}

	while(*szSection)
	{
		unsigned int uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		AstfieldId astfieldId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("astfields.ini").c_str(), szSection, "id", "0");
		CAstfield::CreationParameters creationParms;

		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("astfields.ini").c_str(), szSection, "name", "", szStringBuffer, uiStringLen);
		creationParms.szName = std::string(szStringBuffer);
		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("astfields.ini").c_str(), szSection, "description", "", szStringBuffer, uiStringLen);
		creationParms.szDescription = std::string(szStringBuffer);
		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("astfields.ini").c_str(), szSection, "type", "", szStringBuffer, uiStringLen);
		std::string szMatType(szStringBuffer);
		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		creationParms.matType = IMaterialClass::get_material_type_from_nick(szMatType);

		creationParms.flDropProbability = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("astfields.ini").c_str(), szSection, "drop_probability", "0");
		creationParms.uiMinDropQuantity = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("astfields.ini").c_str(), szSection, "drop_min_quantity", "1");
		creationParms.uiMaxDropQuantity = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("astfields.ini").c_str(), szSection, "drop_max_quantity", "1");
		creationParms.flPirateAttackChancePerTick = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("astfields.ini").c_str(), szSection, "pirate_attack_chance_per_tick", "0");
		creationParms.flRadius = CGameDataManager::read_ini_float(CGameDataManager::get_full_data_file_path("astfields.ini").c_str(), szSection, "radius", "1000");

		uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");
		CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("mat_categories.ini").c_str(), szSection, "attribute_ranges", "", szStringBuffer, uiStringLen);
		std::stringstream ss(szStringBuffer);
		CharacterId charId;
		while(ss >> charId)
		{
			creationParms.vPirateEncounters.push_back(charId);
		}

		pUniverse->add_astfield(astfieldId, creationParms);

		szSection += strlen(szSection) + 1;
	}
}

void CGameDataManager::rough_touch_file(std::string const &szFile)
{
	FILE* f = fopen(szFile.c_str(), "w");
	fclose(f);
}

std::string CGameDataManager::get_full_data_file_path(std::string const &relpath)
{
	std::string fullpath;

	SG::get_game_settings()->get_setting<std::string>("data_directory", fullpath);

	fullpath.append("\\");
	
	fullpath.append(relpath);

	return fullpath;
}

bool CGameDataManager::get_directory_exists(std::string const &szDirectoryPath)
{
	WIN32_FIND_DATAA findData;

	HANDLE hFile = FindFirstFileA(szDirectoryPath.c_str(), &findData);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		FindClose(hFile);

		if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			return true;
		}

		return false;
	}
}

/*
* load all dialogue from dialogue.ini
*/
void CGameDataManager::load_all_dialogue(CDialogueManager* pDialogueManager)
{
	char szSectionNamesBuffer[LOADER_MAX_SECTIONS_BUFFER_SIZE] = "";
	unsigned int uiSectionNamesLen = LOADER_MAX_SECTIONS_BUFFER_SIZE;

	char szStringBuffer[LOADER_MAX_VALUE_BUFFER_SIZE];

	ReadWriteIniKeyValueStringA(nullptr, nullptr, nullptr, CGameDataManager::get_full_data_file_path("dialogue.ini").c_str(), false, szSectionNamesBuffer, &uiSectionNamesLen);

	char* szSection = szSectionNamesBuffer;

	if(strlen(szSection) == 0)
	{
		throw SGException("dialogue.ini doesn\'t exist, or contains no sections");
	}

	while(*szSection)
	{
		unsigned int uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
		strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

		if(strncmp(szSection, "node", 4) == 0)
		{
			DialogueLineId lineId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("dialogue.ini").c_str(), szSection, "id", "0");

			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("dialogue.ini").c_str(), szSection, "character", "", szStringBuffer, uiStringLen);
			std::string szCharacterName(szStringBuffer);
			uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("dialogue.ini").c_str(), szSection, "text", "", szStringBuffer, uiStringLen);
			std::string szLineText(szStringBuffer);
			uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("dialogue.ini").c_str(), szSection, "responses", "", szStringBuffer, uiStringLen);

			std::vector<DialogueResponseId> vResponses;
			DialogueResponseId responseId;
			std::stringstream ss(szStringBuffer);
			while(ss >> responseId)
			{
				vResponses.push_back(responseId);
			}

			uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

			CDialogueLine* pLine = new CDialogueLine(lineId, szCharacterName, szLineText, vResponses);

			pDialogueManager->add_dialogue_line(lineId, pLine);
		}
		else if(strncmp(szSection, "response", 8) == 0)
		{
			DialogueResponseId responseId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("dialogue.ini").c_str(), szSection, "id", "0");

			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("dialogue.ini").c_str(), szSection, "text", "", szStringBuffer, uiStringLen);
			std::string szResponseText(szStringBuffer);
			uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

			DialogueLineId nextLineId = CGameDataManager::read_ini_uint(CGameDataManager::get_full_data_file_path("dialogue.ini").c_str(), szSection, "next_node", "0");

			CGameDataManager::read_ini_string(CGameDataManager::get_full_data_file_path("dialogue.ini").c_str(), szSection, "variable_set", "", szStringBuffer, uiStringLen);

			std::string szVariableKey,szVariableValue;
			std::stringstream ss(szStringBuffer);
			
			if(ss >> szVariableKey && ss >> szVariableValue)
			{ }
			else
			{
				szVariableKey = std::string("");
				szVariableValue = std::string("");
			}

			uiStringLen = LOADER_MAX_VALUE_BUFFER_SIZE;
			strcpy_s(szStringBuffer, LOADER_MAX_VALUE_BUFFER_SIZE, "");

			CDialogueResponse* pResponse = new CDialogueResponse(responseId, szResponseText, nextLineId, szVariableKey, szVariableValue);

			pDialogueManager->add_dialogue_response(responseId, pResponse);
		}
		
		szSection += strlen(szSection) + 1;
	}
}