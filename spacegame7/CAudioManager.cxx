#include "CAudioManager.hxx"
#include "SGLib.hxx"

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