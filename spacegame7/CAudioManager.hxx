#pragma once

#include <list>
#include <mutex>
#include <vector>
#include "SFMLIncludes.hxx"
#include "Defs.hxx"
#include "Util.hxx"
#include "EngineProtos.hxx"
#include "ITransientStateStructure.hxx"

#define ACTIVE_SOUND_LIMIT 256
#define SOUND_BOUND 100000.0f

typedef unsigned int SoundInstanceId;

class CAudioManager : public ITransientStateStructure
{
public:
	CAudioManager()
	{
		sf::Listener::setGlobalVolume(80.0f);
	};
	~CAudioManager()
	{};

	void load_sound_from_file(SoundId const, std::string const, float const);
	SoundInstanceId play_sound(SoundId const);
	void stop_sound(SoundInstanceId const);

	SoundInstanceId play_sound_spatialized(SoundId const, Vector2f const, float const = 1.0f, float const = 100.0f);

	void do_sounds(void);
	void set_listener_point(Vector2f const);

	size_t get_num_active_sounds(void)
	{
		std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

		return this->m_lSounds.size();
	};

	virtual void shifting_out(void) final;

private:
	std::mutex m_mFieldAccess;
	std::vector<sf::SoundBuffer> m_vSoundBuffers;
	std::vector<float> m_vVolumes;
	std::list<sf::Sound> m_lSounds;
	std::list<sf::Sound> m_lQueuedSounds;

	sf::Listener m_soundListener;
};