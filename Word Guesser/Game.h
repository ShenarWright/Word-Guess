#pragma once
#include "pch.h"
#include "SoundManager.h"

class Game
{
public:

	Game(tgui::Gui* gui,SoundManager* sounds,int level = 1);
	~Game();

	void update();
	void render(sf::RenderTarget& target);
	bool isGameRunning();

private:
	tgui::Group::Ptr hud;
	tgui::Group::Ptr win;
	tgui::Group::Ptr lose;
	tgui::Gui* guiptr;
	tgui::Picture::Ptr picture;

	std::mutex guard;

	tgui::Label::Ptr label;
	int level;
	int stars;

	bool running;
	int y_axis;

	std::vector<std::vector<tgui::EditBox::Ptr>>rows;
	std::vector<std::string> words;
	Json::Value pathDatabase;
	Json::Value levelDatabase;


	std::string imgpath;
	std::string word;
	std::string guess;
	std::map<char, int > wordDuplication;


	sf::Clock wintimer;
	sf::Clock keyignoretimer;
	bool won;
	char ignoredkey;

	SoundManager* sounds_ptr;

	tgui::Texture colors[5];
	tgui::Texture textures[3];

	void parseDuplication();

	void createHud();
	void createRow(int y);

	bool checkWord();

	void clearSlate();
	void rebuildSlate();
	void getWords();
	void selectWord();

	void saveDatabase();
	void loadLevelDatabase();
	char tolower(char a);
};

