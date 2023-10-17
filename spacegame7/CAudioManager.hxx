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
typedef unsigned int MusicId;

class CAudioManager : public ITransientStateStructure
{
public:
	CAudioManager();
	~CAudioManager();

	void load_sound_from_file(SoundId const, std::string const, float const);
	SoundInstanceId play_sound(SoundId const);
	void stop_sound(SoundInstanceId const);

	void load_music_from_file(MusicId const, std::string const);
	void play_music(MusicId const, float const, bool const);
	void stop_music(void);

	SoundInstanceId play_sound_spatialized(SoundId const, Vector2f const, float const = 1.0f, float const = 100.0f);

	void do_sounds(void);
	void set_listener_point(Vector2f const);

	size_t get_num_active_sounds(void)
	{
		std::lock_guard<std::mutex> lock(this->m_mFieldAccess);

		return this->m_lSounds.size();
	};

	virtual void shifting_out(void) final;

	void set_music_global_volume(float const);

	void set_ambient_music(MusicId const);
	void set_battle_music(MusicId const);

	void entered_battle(void);
	void exited_battle(void);

	void tick_music(float const);

private:
	std::mutex m_mFieldAccess;
	std::vector<sf::SoundBuffer> m_vSoundBuffers;
	std::vector<float> m_vVolumes;
	std::list<sf::Sound> m_lSounds;
	std::list<sf::Sound> m_lQueuedSounds;

	MusicId m_uiAmbientMusic;
	MusicId m_uiBattleMusic;

	sf::Music* m_currentMusicTrack;
	sf::Music* m_nextMusicTrack;
	std::vector<std::string> m_vMusicTracks;

	bool m_bInBattle;

	sf::Listener m_soundListener;

	void enqueue_music(std::string const&);

	float m_flMusicVolume;
	bool m_bFading;
	float m_flFadeTime;
};