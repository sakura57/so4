#include "CAudioManager.hxx"
#include "SGLib.hxx"

#define MUSIC_FADE_TIME 2.5f

CAudioManager::CAudioManager()
	: m_uiAmbientMusic(0), m_uiBattleMusic(0), m_bInBattle(false), m_currentMusicTrack(nullptr), m_flMusicVolume(100.0f), m_nextMusicTrack(nullptr), m_bFading(false), m_flFadeTime(0.0f)
{

}

CAudioManager::~CAudioManager()
{
	if(this->m_currentMusicTrack)
	{
		delete this->m_currentMusicTrack;
	}

	if(this->m_nextMusicTrack)
	{
		delete this->m_nextMusicTrack;
	}
}

void CAudioManager::load_sound_from_file(SoundId const uiSoundId, std::string const szSoundFile, float const fVolume)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	std::string szFullSoundFilePath = SG::get_game_data_manager()->get_full_data_file_path(std::string("sound\\") + szSoundFile);

	if(this->m_vSoundBuffers.size() <= uiSoundId)
	{
		this->m_vSoundBuffers.resize(uiSoundId + 1);
		this->m_vVolumes.resize(uiSoundId + 1);
	}

	this->m_vSoundBuffers[uiSoundId].loadFromFile(szFullSoundFilePath);
	this->m_vVolumes[uiSoundId] = fVolume;
}

void CAudioManager::load_music_from_file(MusicId const uiMusicId, std::string const szSoundFile)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	std::string szFullSoundFilePath = SG::get_game_data_manager()->get_full_data_file_path(std::string("music\\") + szSoundFile);

	if(this->m_vMusicTracks.size() <= uiMusicId)
	{
		this->m_vMusicTracks.resize(uiMusicId + 1);
	}

	this->m_vMusicTracks[uiMusicId] = szFullSoundFilePath;
}

void CAudioManager::play_music(MusicId const uiMusicId, float const flVolume, bool const bLoop)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(uiMusicId >= this->m_vMusicTracks.size())
	{
		return;
	}

	this->enqueue_music(this->m_vMusicTracks[uiMusicId]);
}

void CAudioManager::stop_music(void)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(this->m_currentMusicTrack->getStatus() == sf::Music::Playing)
	{
		this->m_currentMusicTrack->stop();
	}
}

SoundInstanceId CAudioManager::play_sound(SoundId const uiSoundId)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	this->m_lQueuedSounds.emplace_back(this->m_vSoundBuffers[uiSoundId]);
	sf::Sound &sound = this->m_lQueuedSounds.back();

	sound.setRelativeToListener(true);
	sound.setPosition(0.0f, 0.0f, 0.0f);
	sound.setVolume(this->m_vVolumes[uiSoundId]);

	return this->m_lQueuedSounds.size() - 1;
}

SoundInstanceId CAudioManager::play_sound_spatialized(
	SoundId const uiSoundId,
	Vector2f const vPosition,
	float const fAttenuation,
	float const fMinDistance)
{
	if (this->m_lSounds.size() > ACTIVE_SOUND_LIMIT)
	{
		return 0;
	}

	if(abs(vPosition.x) > SOUND_BOUND || abs(vPosition.y) > SOUND_BOUND ||
		std::isnan(vPosition.x) || std::isnan(vPosition.y))
	{
		return 0;
	}

	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	this->m_lQueuedSounds.emplace_back(this->m_vSoundBuffers[uiSoundId]);
	sf::Sound &sound = this->m_lQueuedSounds.back();

	sound.setRelativeToListener(false);
	sound.setPosition(vPosition.x, vPosition.y, 0.0f);
	sound.setAttenuation(fAttenuation);
	sound.setMinDistance(fMinDistance);
	sound.setVolume(this->m_vVolumes[uiSoundId]);

	return this->m_lQueuedSounds.size() - 1;
}

void CAudioManager::do_sounds(void)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	/*
	 * iterate sounds, if they have finished playing,
	 * remove them from the sound list
	 */
	std::list<sf::Sound>::iterator i = this->m_lSounds.begin();

	while(i != this->m_lSounds.end())
	{
		if(i->getStatus() != sf::Sound::Playing)
		{
			this->m_lSounds.erase(i++);
		}
		else
		{
			++i;
		}
	}

	/*
	 * play queued sounds, and move them to the sound list
	 */
	if(this->m_lQueuedSounds.size())
	{
		for(sf::Sound &sound : this->m_lQueuedSounds)
		{
			sound.play();
		}

		this->m_lSounds.splice(this->m_lSounds.end(), this->m_lQueuedSounds);
	}
}

void CAudioManager::set_listener_point(Vector2f const vPosition)
{
	sf::Listener::setPosition(vPosition.x, vPosition.y, 0.0f);
}

void CAudioManager::shifting_out(void)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	this->m_lQueuedSounds.clear();
	this->m_lSounds.clear();
	this->m_soundListener.setPosition(sf::Vector3f(0.0f, 0.0f, 0.0f));

	if(this->m_currentMusicTrack)
	{
		if(this->m_currentMusicTrack->getStatus() == sf::Music::Playing)
		{
			this->m_currentMusicTrack->stop();
		}

		delete this->m_currentMusicTrack;

		this->m_currentMusicTrack = nullptr;
	}

	if(this->m_nextMusicTrack)
	{
		if(this->m_nextMusicTrack->getStatus() == sf::Music::Playing)
		{
			this->m_nextMusicTrack->stop();
		}

		delete this->m_nextMusicTrack;

		this->m_nextMusicTrack = nullptr;
	}

	this->m_bFading = false;
	this->m_flFadeTime = 0.0f;
}

void CAudioManager::stop_sound(SoundInstanceId const uiSoundId)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(uiSoundId >= this->m_lSounds.size())
	{
		return;
	}

	auto i = this->m_lSounds.begin();
	std::advance(i, uiSoundId);

	i->stop();
}

void CAudioManager::set_ambient_music(MusicId const uiMusicId)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(uiMusicId >= this->m_vMusicTracks.size())
	{
		return;
	}

	this->m_uiAmbientMusic = uiMusicId;

	if(this->m_bInBattle == false)
	{
		if(uiMusicId > 0)
		{
			this->enqueue_music(this->m_vMusicTracks[uiMusicId]);
		}
	}
}

void CAudioManager::set_battle_music(MusicId const uiMusicId)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(uiMusicId >= this->m_vMusicTracks.size())
	{
		return;
	}

	this->m_uiBattleMusic = uiMusicId;

	if(this->m_bInBattle == true)
	{
		if(uiMusicId > 0)
		{
			this->enqueue_music(this->m_vMusicTracks[uiMusicId]);
		}
	}
}

void CAudioManager::entered_battle(void)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(this->m_bInBattle == false)
	{
		this->m_bInBattle = true;

		if(this->m_uiBattleMusic > 0)
		{
			this->enqueue_music(this->m_vMusicTracks[this->m_uiBattleMusic]);
		}
	}
}

void CAudioManager::exited_battle(void)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(this->m_bInBattle == true)
	{
		this->m_bInBattle = false;

		if(this->m_uiAmbientMusic > 0)
		{
			this->enqueue_music(this->m_vMusicTracks[this->m_uiAmbientMusic]);
		}
	}
}

void CAudioManager::set_music_global_volume(float const flMusicVolume)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	this->m_flMusicVolume = flMusicVolume;
}

void CAudioManager::tick_music(float const flDelta)
{
	std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

	if(this->m_bFading)
	{
		if(this->m_flFadeTime < MUSIC_FADE_TIME)
		{
			float flNextMusicVolume = this->m_flFadeTime / MUSIC_FADE_TIME * this->m_flMusicVolume;
			float flCurrentMusicVolume = this->m_flMusicVolume - flNextMusicVolume;

			if(this->m_currentMusicTrack && this->m_currentMusicTrack->getStatus() == sf::Music::Playing)
			{
				this->m_currentMusicTrack->setVolume(flCurrentMusicVolume);
			}

			this->m_nextMusicTrack->setVolume(flNextMusicVolume);
		}
		else
		{
			if(this->m_currentMusicTrack)
			{
				if(this->m_currentMusicTrack->getStatus() == sf::Music::Playing)
				{
					this->m_currentMusicTrack->stop();
				}

				delete this->m_currentMusicTrack;
			}

			this->m_currentMusicTrack = this->m_nextMusicTrack;
			this->m_currentMusicTrack->setVolume(this->m_flMusicVolume);

			this->m_nextMusicTrack = nullptr;

			this->m_bFading = false;
		}

		this->m_flFadeTime += flDelta;
	}
}

void CAudioManager::enqueue_music(std::string const &musicFile)
{
	if(this->m_nextMusicTrack != nullptr)
	{
		if(this->m_nextMusicTrack->getStatus() == sf::Music::Playing)
		{
			this->m_nextMusicTrack->stop();
		}

		delete this->m_nextMusicTrack;
	}

	this->m_nextMusicTrack = new sf::Music;

	if(!this->m_nextMusicTrack->openFromFile(musicFile))
	{
		throw SGException("Failed to open music file");
	}

	this->m_nextMusicTrack->setLoop(true);
	this->m_nextMusicTrack->setVolume(0.0f);
	this->m_nextMusicTrack->play();

	this->m_bFading = true;
	this->m_flFadeTime = 0.0f;
}