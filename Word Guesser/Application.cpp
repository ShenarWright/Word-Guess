#include "Application.h"
#include "Utils.h"

Application::Application() : 
	shouldrungame(false)
{
	initWindow();
	initGui();

	loadMainMenu();
	loadLevelSelection();
	loadHelpMenu();
	loadSettings();
}

Application::~Application()
{
	delete game;
}

void Application::run()
{
	while (window.isOpen())
	{
		pollEvents();
		update();
		render();
	}
}

void Application::initWindow()
{
	std::fstream fs;
	fs.open("config/window.json");

	if (fs.is_open())
	{
		std::string buffer;
		std::string temp;
		while (std::getline(fs, temp))
		{
			buffer += temp;
		}

		Json::Reader reader;

		reader.parse(buffer, windowdata);

		window.create(sf::VideoMode( windowdata["windowsize"][0].asInt() ,windowdata["windowsize"][1].asInt() ), windowdata["windowname"].asString());
		window.setFramerateLimit(windowdata["fps"].asInt());
	}
	else
	{
		window.create(sf::VideoMode(1600, 900), "Wordle Guesser");
		window.setFramerateLimit(30);
	}

	fs.close();

	icon.loadFromFile("icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void Application::initGui()
{
	gui.setTarget(window);
	handleResize();
}

void Application::pollEvents()
{
	while (window.pollEvent(ev))
	{
		switch (ev.type)
		{
		case sf::Event::Closed:
			window.close();
		break;
		case sf::Event::Resized:
			handleResize();
		break;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11) && fullscreentimer.getElapsedTime().asSeconds() > .5f && window.hasFocus())
		{
			if (fullscreen)
			{
				window.create(sf::VideoMode(windowdata["windowsize"][0].asInt(), windowdata["windowsize"][1].asInt()), windowdata["windowname"].asString());
				window.setFramerateLimit(windowdata["fps"].asInt());
				handleResize();
				fullscreen = false;
			}
			else
			{
				window.create(sf::VideoMode::getDesktopMode(), windowdata["windowname"].asString(), sf::Style::Fullscreen);
				window.setFramerateLimit(windowdata["fps"].asInt());
				handleResize();
				fullscreen = true;
			}
			fullscreentimer.restart();
		}

		gui.handleEvent(ev);
	}
}

void Application::update()
{
	if (window.hasFocus())
	{
		if (game != nullptr)
		{
			if (!game->isGameRunning())
			{
				stopGame();
			}
			else
				game->update();
		}
	}
}

void Application::render()
{
	window.clear();
	
	if (game != nullptr)
	{
		game->render(window);
	}

	gui.draw();
	window.display();
}

void Application::loadMainMenu()
{
	mainMenu = tgui::Group::create();
	mainMenu->loadWidgetsFromFile("home.txt");

	auto start = mainMenu->get<tgui::Button>("PLAY");
	auto settings = mainMenu->get<tgui::Button>("settings button");
	auto help = mainMenu->get<tgui::Button>("HELP");
	auto quit = mainMenu->get<tgui::Button>("QUIT");

	start->onClick([&]()
	{
			mainMenu->setVisible(false);
			levelSelection->setVisible(true);

		sounds.playtrack(SoundManager::PLAYBUTTONSOUND);
	});

	settings->onClick([&](tgui::Button::Ptr btn)
	{
		sounds.playtrack(SoundManager::PLAYBUTTONSOUND);
		if (!settingsMenu->isVisible())
		{
			settingsMenu->setVisible(true);
			mainMenu->setVisible(false);
		}
		else
		{
			settingsMenu->setVisible(false);
			mainMenu->setVisible(true);
		}
		btn->setVisible(true);
	},settings);

	help->onClick([&]()
	{
		sounds.playtrack(SoundManager::PLAYBUTTONSOUND);
		mainMenu->setVisible(false);
		helpMenu->setVisible(true);
	});

	quit->onClick([&]() 
	{
		sounds.playtrack(SoundManager::PLAYBUTTONSOUND);
		window.close(); 
	});

	gui.add(mainMenu);
}

void Application::loadLevelSelection()
{
	loadLevelData();

	levelSelection = tgui::Group::create();

	try
	{
		levelSelection->loadWidgetsFromFile("levels.txt");
	}
	catch (tgui::Exception& e)
	{
		std::cout << e.what() << '\n';
	}

	levelSelection->setVisible(false);

	auto back = levelSelection->get<tgui::Button>("BACK BUTTON");
	back->onClick([&]()
		{
			mainMenu->setVisible(true);
			levelSelection->setVisible(false);
			sounds.playtrack(SoundManager::DELETESOUND);
		});

	gui.add(levelSelection);


	auto sPanel = levelSelection->get<tgui::ScrollablePanel>("ScrollablePanel1");
	auto button = levelSelection->get<tgui::Button>("LEVEL SELECT BUTTON");
	auto number = levelSelection->get<tgui::Label>("LEVEL NUMBER");
	auto star1 = levelSelection->get<tgui::Picture>("FIRST STAR");
	auto star2 = levelSelection->get<tgui::Picture>("SECOND STAR");

	button->setVisible(false);
	number->setVisible(false);
	star1->setVisible(false);
	star2->setVisible(false);

	number->setAutoSize(false);
	number->setEnabled(false);

	int x = 0;
	int y = 0;

	int Xspace = 220;
	int Yspace = 290;

	for (int i = 0; i < levelData["maxlevel"].asInt(); i++)
	{
		auto btn1 = tgui::Button::copy(button);
		auto num1 = tgui::Label::copy(number);
		auto st1 = tgui::Picture::copy(star1);
		auto st2 = tgui::Picture::copy(star2);

		btn1->setPosition(button->getPosition().x + (x * Xspace), button->getPosition().y + (y * Yspace));
		btn1->setVisible(true);

		std::stringstream ss;
		ss << i + 1;

		num1->setPosition(number->getPosition().x + (x * Xspace), number->getPosition().y + (y * Yspace));
		num1->setVisible(true);
		num1->setText(ss.str());
		num1->ignoreMouseEvents(true);

		if (i >= levelData["currentlevel"].asInt())
			btn1->setEnabled(false);
		else
		{
			if (levelData["stars"][i].asInt() == 1)
			{
				st1->setPosition(star1->getPosition().x + (x * Xspace), star1->getPosition().y + (y  * Yspace));
				st1->setVisible(true);
			}
			if (levelData["stars"][i].asInt() == 2)
			{
				st1->setPosition(star1->getPosition().x + (x * Xspace), star1->getPosition().y + (y * Yspace));
				st1->setVisible(true);

				st2->setPosition(star2->getPosition().x + (x * Xspace), star2->getPosition().y + (y * Yspace));
				st2->setVisible(true);
			}

			if (levelData["stars"][i].asInt() == 3)
			{
				st1->setVisible(false);
				st2->setVisible(false);
				btn1->getRenderer()->setTexture("res/THREE STAR WINNER.png");
			}
		}

		btn1->onClick([&](int level)
		{
				levelSelection->setVisible(false);
				sounds.playtrack(SoundManager::DELETESOUND);
				auto picture = tgui::Picture::create("res/WORD GUESS LOADING SCREEN.png");
				gui.add(picture);
				startGame(level);
				gui.remove(picture);
				shouldrungame = true;
		},i);

		number->setText(ss.str());

		x++;
		if (x + 1 >= 9)
		{
			y++;
			x = 0;
		}


		sPanel->add(btn1);
		sPanel->add(num1);
		sPanel->add(st1);
		sPanel->add(st2);
	}
}

void Application::loadHelpMenu()
{
	helpMenu = tgui::Group::create();
	helpMenu->setVisible(false);
	gui.add(helpMenu);

	auto back = tgui::Button::create("back");
	back->setSize(100, 100);
	back->onClick([&]()
		{
			helpMenu->setVisible(false);
			mainMenu->setVisible(true);
		});

	auto pic = tgui::Picture::create("res/tutorial/1.png");
	pic->setSize("75%");
	pic->setPosition("10%");

	helpMenu->add(pic);
	helpMenu->add(back);

}

void Application::loadSettings()
{
	settingsMenu = tgui::Group::create();
	settingsMenu->setVisible(false);

	try 
	{
		settingsMenu->loadWidgetsFromFile("settings.txt");
	}
	catch (tgui::Exception& e)
	{
		std::cout << e.what() << '\n';
	}

	auto music = settingsMenu->get<tgui::ToggleButton>("MUSIC BUTTON");
	auto sound = settingsMenu->get<tgui::ToggleButton>("SOUND BUTTON");

	music->setDown(sounds.isMusicMute());
	sound->setDown(sounds.isSoundMute());

	auto soundSlider = settingsMenu->get<tgui::Slider>("SOUND SLIDER");
	auto musicSlider = settingsMenu->get<tgui::Slider>("MUSIC SLIDER");

	soundSlider->setValue(sounds.getSoundVolume());
	musicSlider->setValue(sounds.getMusicVolume());

	music->onClick([&](tgui::ToggleButton::Ptr musicbtn)
		{
			sounds.muteMusic(musicbtn->isDown());
			sounds.playtrack(SoundManager::PLAYBUTTONSOUND);
		},music);

	sound->onClick([&](tgui::ToggleButton::Ptr soundbtn)
		{
			sounds.muteSound(soundbtn->isDown());
			sounds.playtrack(SoundManager::PLAYBUTTONSOUND);
		},sound);

	soundSlider->onValueChange([&](tgui::Slider::Ptr sSlider,tgui::ToggleButton::Ptr soundbtn)
		{
			sounds.setSoundVolume(sSlider->getValue());
			sounds.muteSound(false);
			soundbtn->setDown(false);

			if (sSlider->getValue() == 0) soundbtn->setDown(true);
		},soundSlider,sound);

	musicSlider->onValueChange([&](tgui::Slider::Ptr sSlider, tgui::ToggleButton::Ptr musicbtn)
		{
			sounds.setMusicVolume(sSlider->getValue());
			sounds.muteMusic(false);
			musicbtn->setDown(false);

			if (sSlider->getValue() == 0) musicbtn->setDown(true);
		},musicSlider,music);

	auto back = tgui::Button::create("Back");
	back->setSize(100, 100);

	back->onClick([&]()
		{
			mainMenu->setVisible(true);
			settingsMenu->setVisible(false);
		});
	
	settingsMenu->add(back);
	gui.add(settingsMenu);
}

void Application::handleResize()
{
	gui.setRelativeView({ 0,0,1920.f / (float)window.getSize().x,1080.f / (float)window.getSize().y});
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void Application::startGame(int level)
{
	mainMenu->setVisible(false);
	game = new Game(&gui,&sounds,level);
	levelSelection->removeAllWidgets();
}

void Application::stopGame()
{
	delete game;
	game = nullptr;
	mainMenu->setVisible(true);
	loadLevelSelection();
}

void Application::loadLevelData()
{
	std::fstream fs;
	fs.open("data/level.json");

	if (fs.is_open())
	{
		std::string buffer;
		std::string temp;

		while(std::getline(fs, temp))
		{
			buffer += utils::decrypt(temp);
		}

		Json::Reader reader;
		reader.parse(buffer, levelData);
	}

	fs.close();
}
