#pragma once

#include <string>

#define GAME_TITLE_STRING "Spaced Out 4"
#define GAME_VERSION_STRING "PRE-ALPHA"

//interface is defined as alias of struct (all members public by default)
#if !defined(interface)
#define interface struct
#endif

#if defined(__unix__)
#define INLINE __attribute__((always_inline))
#elif defined(_WIN32) || defined(WIN32)
#define INLINE __forceinline
#endif

#define GAME_INITIAL_DELTA 0.016667f

/*
 * general-purpose exception class.
 * for now just encapsulates a std::string
 */
class SGException
{
public:
	SGException(void) = delete;

	SGException(std::string const &szException)
	{
		this->m_szException = szException;
	};

	std::string get_exception(void) const
	{
		return this->m_szException;
	};

private:
	std::string m_szException;
};

//#define DEFAULT_WINDOW_WIDTH 1280
//#define DEFAULT_WINDOW_HEIGHT 720
#define DEFAULT_WINDOW_HEIGHT CGameDataManager::get_setting_intransient<unsigned int>("screen_height")
#define DEFAULT_WINDOW_WIDTH CGameDataManager::get_setting_intransient<unsigned int>("screen_width")
#define DEFAULT_WINDOW_FULLSCREEN (CGameDataManager::get_setting_intransient<unsigned int>("fullscreen") != 0)
#define DEFAULT_WINDOW_NAME "Spaced Out 4"