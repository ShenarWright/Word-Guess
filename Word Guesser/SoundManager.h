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

private:
	std::vector<sf::SoundBuffer>Sounds;
	sf::Sound ambient;
	sf::Sound s;
};

