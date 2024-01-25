#include "SoundManager.h"

SoundManager::SoundManager():
	musicVolume(0),soundVolume(0)
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

	std::string buffer = utils::loadEntireFile("data/music.json");
	Json::Reader reader;
	reader.parse(buffer, musicData);

	musicVolume = musicData["music"]["volume"].asInt();
	soundVolume = musicData["sound"]["volume"].asInt();


	ambient.setLoop(true);
	ambient.setBuffer(Sounds[3]);
	ambient.setVolume(musicVolume);
	ambient.play();

	if (musicData["music"]["isMute"].asBool()) muteMusic();
	if (musicData["sound"]["isMute"].asBool()) muteSound();
}

SoundManager::~SoundManager()
{
	musicData["music"]["isMute"] = isMusicMute();
	musicData["sound"]["isMute"] = isSoundMute();

	musicData["music"]["volume"] = musicVolume;
	musicData["sound"]["volume"] = soundVolume;

	Json::StyledWriter writer;
	std::string buffer = writer.write(musicData);
	utils::saveFile("data/music.json",buffer);
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

void SoundManager::setMusicVolume(float volume)
{
	musicVolume = volume;
	ambient.setVolume(musicVolume);
}

void SoundManager::setSoundVolume(float volume)
{
	soundVolume = volume;
	s.setVolume(soundVolume);
}

void SoundManager::muteSound(bool mute)
{
	if (mute)
	{
		std::cout << "Sound mute\n";
		s.setVolume(0);
	}
	else
	{
		s.setVolume(soundVolume);
	}
}

void SoundManager::muteMusic(bool mute)
{
	if (mute)
	{
		std::cout << "Music mute\n";
		ambient.setVolume(0);
	}
	else
	{
		ambient.setVolume(musicVolume);
	}
}



bool SoundManager::isSoundMute()
{
	if (s.getVolume() == 0) return true;
	return false;
}

bool SoundManager::isMusicMute()
{
	if (ambient.getVolume() == 0) return true;
	return false;
}

float SoundManager::getSoundVolume()
{
	return soundVolume;
}

float SoundManager::getMusicVolume()
{
	return musicVolume;
}
