#pragma once

#include "pch.h"

class SoundManager
{
public:
	SoundManager();

	void update();

	enum Sound {
		DELETESOUND = 0,
		KEYPRESSSOUND,
		PLAYBUTTONSOUND,
		AMBIENTMUSIC,
		WINMUSIC,
		LOSESOUND,
		LOSESCREENSOUND
	};
	void playtrack(Sound track, bool loop = false);

	void setMusicVolume(float volume);
	void setSoundVolume(float volume);

	void muteSound(bool mute = true);
	void muteMusic(bool mute = true);
private:
	std::vector<sf::SoundBuffer>Sounds;
	sf::Sound ambient;
	sf::Sound s;

	float musicVolume;
	float soundVolume;

	float oldMusicVolume;
	float oldSoundVolume;
};

