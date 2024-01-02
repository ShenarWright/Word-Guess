#include "SoundManager.h"

SoundManager::SoundManager()
{
	std::string paths[] =
	{
		"res/audio/Delete Sound.opus",
		"res/audio/Keypress Sound.ogg",
		"res/audio/Play Button Press Sound.ogg",
		"res/audio/Word Guess Music.ogg",
		"res/audio/Word Guess Victory Music.opus",
		"res/audio/Lose sound.opus",
		"res/audio/Lose screen sound.opus"
	};

	for (int i = 0; i < 7; i++)
	{
		Sounds.push_back(sf::SoundBuffer());
		Sounds.back().loadFromFile(paths[i]);
	}

	//sf::Sound s;
	ambient.setLoop(true);
	ambient.setBuffer(Sounds[3]);
	ambient.play();
	//musics[0].play();
	//musics[0].setLoop(true);
}

void SoundManager::update()
{

}

void SoundManager::playtrack(Sound track, bool loop)
{
	if (track == Sound::WINMUSIC)
	{
		ambient.setLoop(true);
		ambient.resetBuffer();
		ambient.setBuffer(Sounds[track]);
		ambient.play();

		//musics[0].pause();
		return;
	}

	if (track == Sound::AMBIENTMUSIC)
	{
		//musics[0].play();
		//musics[Sound::WINMUSIC].stop();
		if (ambient.getBuffer() == &Sounds[track])
			return;

		ambient.setLoop(true);
		ambient.setBuffer(Sounds[track]);
		ambient.play();

		return;
	}

	if (track == Sound::LOSESCREENSOUND)
	{
		ambient.setLoop(false);
		ambient.resetBuffer();
		ambient.setBuffer(Sounds[track]);
		ambient.play();

		//musics[0].pause();
		return;
	}

	s.setBuffer(Sounds[track]);
	s.play();
	
	//musics[track].setLoop(loop);
	//musics[track].play();

}
