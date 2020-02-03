#pragma once

#include <string>
#include <list>
#include <random>
#include <mutex>
#include <future>

#include "Archetype.hxx"
#include "Loadout.hxx"
#include "Character.hxx"
#include "Faction.hxx"
#include "Pilot.hxx"

#include "CRandomNameGenerator.hxx"
#include "CAudioManager.hxx"
#include "IRenderPipeline.hxx"
#include "IStaticTextureManager.hxx"
#include "CParticleManager.hxx"
#include "IUniverse.hxx"
#include "ILootTableManager.hxx"
#include "CSkillDataManager.hxx"
#include "IMatManager.hxx"

/*
 * This class is only PARTIALLY threadsafe.
 * 
 * All functions prefixed with load_ are NOT threadsafe.
 * Loading occurs before the main thread spawns any other
 * threads, so they assume they are running from a single-
 * threaded environment.
 * 
 * All other public functions are threadsafe.
 */
class CGameDataManager
{
public:
	//static class, non-instantiable
	CGameDataManager(void);
	~CGameDataManager(void);

	static void load_all_graphics_assets(IRenderPipeline *);
	static void load_all_static_graphics_assets(IStaticTextureManager *);
	static void load_all_audio_assets(CAudioManager *);
	static void load_all_animation_assets(CParticleManager *);
	static void load_all_sectors(IUniverse *);
	static void load_all_bases(IUniverse *);
	static void load_all_astfields(IUniverse*);
	static void load_all_loottables(ILootTableManager *);
	static void load_all_resource_classes(IMatManager *);

	static std::future<bool> load_from_save(std::string const &);
	static void dump_to_save(std::string const &, BaseId const);

	void load_shiparch_data(void);
	void load_equiparch_data(void);
	void load_loadouts_data(void);
	void load_factions_data(void);
	void load_characters_data(void);
	void load_pilots_data(void);
	void load_namecoll_data(void);
	void load_skill_data(void);

	void save_settings_data(void);

	CSkillDataManager *get_skill_data_manager(void);

	/*
	 * This is a function which returns the value of a setting
	 * which should not be able to change over the course of the game,
	 * e.g. screen resolution.
	 * 
	 * Values are cached.
	 */
	template<class T>
	static T get_setting_intransient(std::string s)
	{
		static std::unordered_map<std::string, T> valMap;

		if (!CGameDataManager::settingsLoaded)
		{
			CGameDataManager::load_settings_data();
			CGameDataManager::settingsLoaded = true;
		}

		std::unordered_map<std::string, T>::iterator i = valMap.find(s);
		if(i != valMap.end())
		{
			return i->second;
		}

		CGameSettings *settings = SG::get_game_settings();
		T k;

		if(settings->get_setting<T>(s, k))
		{
			valMap[s] = k;
			return k;
		}

		return T();
	};

	/*
	* Return an archetype pointer for a given arch ID.
	*
	* Archetype structs are read-only after game data is read,
	* so there is no need to lock any mutex.
	*/
	template<class T>
	T const * get_arch(ArchId const uiArchId)
	{
		for (Archetype *pArch : this->m_lArch)
		{
			if(pArch->uiArchId == uiArchId)
			{
				if(pArch->uiArchType != T::Type)
				{
					throw SGException("Client expected wrong archetype class");
				}

				return reinterpret_cast<T const*>(pArch);
			}
		}

		return nullptr;
	};

	/*
	* Return an archetype pointer for a given arch ID.
	*/
	template<>
	Archetype const * get_arch<Archetype>(ArchId const uiArchId)
	{
		for(Archetype *pArch : this->m_lArch)
		{
			if(pArch->uiArchId == uiArchId)
			{
				return pArch;
			}
		}

		return nullptr;
	};

	Loadout const *get_loadout(LoadoutId const uiLoadoutId)
	{
		if(uiLoadoutId > m_lLoadouts.size())
		{
			return nullptr;
		}

		return this->m_lLoadouts[uiLoadoutId];
	};

	CNameCollection const *get_name_collection(NameCollectionId const uiNameCollectionId)
	{
		if (uiNameCollectionId > m_lNameCollections.size())
		{
			return nullptr;
		}

		return this->m_lNameCollections[uiNameCollectionId];
	};

	std::string get_random_name(NameCollectionId const uiNameCollectionId);

	Faction const *get_faction(FactionId const uiFactionId)
	{
		if (uiFactionId > m_lFactions.size())
		{
			return nullptr;
		}

		return this->m_lFactions[uiFactionId];
	};

	Character const *get_character(CharacterId const uiCharacterId)
	{
		if (uiCharacterId > m_lCharacters.size())
		{
			return nullptr;
		}

		return this->m_lCharacters[uiCharacterId];
	};

	Pilot const *get_pilot(PilotId const uiPilotId)
	{
		if (uiPilotId > m_lPilots.size())
		{
			return nullptr;
		}

		return this->m_lPilots[uiPilotId];
	};
public:
	static Vector2f read_ini_vector(char const*, char const*, char const*, char const*);
	static unsigned int read_ini_uint(char const*, char const*, char const*, char const*);
	static float read_ini_float(char const*, char const*, char const*, char const*);
	static void read_ini_string(char const*, char const*, char const*, char const*, char*, unsigned int&);

	static std::string get_full_data_file_path(std::string const &);

	static bool get_directory_exists(std::string const&);

private:
	static bool settingsLoaded;

	static void load_settings_data(void);

	static bool load_from_save_delegate(std::string const&);
	static void dump_to_save_delegate(std::string const&, BaseId const);

	static void rough_touch_file(std::string const&);

	void verify_shiparch(ShipArch const &);
	void verify_weaparch(WeaponArch const &);

	std::list<Archetype*> m_lArch;
	std::vector<Loadout const *> m_lLoadouts;
	std::vector<Faction const *> m_lFactions;
	std::vector<Character const *> m_lCharacters;
	std::vector<Pilot const *> m_lPilots;
	std::vector<CNameCollection const *> m_lNameCollections;

	CSkillDataManager* m_pSkillDataManager;
};