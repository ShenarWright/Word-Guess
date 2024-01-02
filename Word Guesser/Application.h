#pragma once

#include "pch.h"

#include "Game.h"
#include "SoundManager.h"

class Application
{
public:
	Application();
	~Application();

	void run();
private:
	sf::RenderWindow window;
	sf::Event ev;

	tgui::Gui gui;
	tgui::Group::Ptr mainMenu;
	tgui::Group::Ptr levelSelection;

	Game* game;
	SoundManager sounds;

	sf::Clock fullscreentimer;

	Json::Value windowdata;
	Json::Value levelData;

	bool fullscreen;
	bool shouldrungame;

	sf::Image icon;

	void initWindow();
	void initGui();

	void pollEvents();
	void update();
	void render();

	void loadMainMenu();
	void loadLevelSelection();

	void handleResize();

	void startGame(int level);
	void stopGame();

	void loadLevelData();

};


