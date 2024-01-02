#include "Application.h"
#include "Utils.h"

Application::Application() : 
	shouldrungame(false)
{
	initWindow();
	initGui();

	loadMainMenu();
	loadLevelSelection();

	std::cout << utils::encrypt("hello{}{}") << '\n';
	std::cout << utils::decrypt("fgjnm{}{}") << '\n';
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

		//Json::Value value;
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
	window.setIcon(1934, 1654, icon.getPixelsPtr());
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

	//auto picture = tgui::Picture::create("res/background.png");
	auto start = mainMenu->get<tgui::Button>("PLAY");
	auto settings = mainMenu->get<tgui::Button>("settings button");
	auto help = mainMenu->get<tgui::Button>("HELP");
	auto quit = mainMenu->get<tgui::Button>("QUIT");

	//start->setSize(200, 150);

	//mainMenu->add(picture);
	//mainMenu->add(start);

	start->onClick([&]()
	{
		//startGame();
			mainMenu->setVisible(false);
			levelSelection->setVisible(true);

		sounds.playtrack(SoundManager::PLAYBUTTONSOUND);
	});

	settings->onClick([&]()
	{
		sounds.playtrack(SoundManager::PLAYBUTTONSOUND);
	});

	help->onClick([&]()
	{
		sounds.playtrack(SoundManager::PLAYBUTTONSOUND);
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

	//auto star3 = levelSelection->get<tgui::Button>("LEVEL SELECT BUTTON"); x: 220 y: 290

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
		//num1->getRenderer()->setBorders(tgui::Borders(3, 3, 3, 3));

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

void Application::handleResize()
{
	gui.setRelativeView({ 0,0,1920.f / (float)window.getSize().x,1080.f / (float)window.getSize().y});
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
			buffer += temp;
		}

		Json::Reader reader;
		reader.parse(buffer, levelData);
	}

	fs.close();
}
