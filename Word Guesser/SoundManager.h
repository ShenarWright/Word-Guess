#pragma once

#include "pch.h"
#include "Utils.h"

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

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

	bool isSoundMute();
	bool isMusicMute();

	float getSoundVolume();
	float getMusicVolume();
private:
	std::vector<sf::SoundBuffer>Sounds;
	sf::Sound ambient;
	sf::Sound s;

	Json::Value musicData;

	float musicVolume;
	float soundVolume;
};

