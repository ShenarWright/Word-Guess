#include "Game.h"

#include "Debug.h"

Game::Game(tgui::Gui* gui, SoundManager* sounds,int level): 
	guiptr(gui), running(true), y_axis(0), level(level),sounds_ptr(sounds),won(false)
{
	hud = tgui::Group::create();
	win = tgui::Group::create();
	lose = tgui::Group::create();

	win->setVisible(false);
	lose->setVisible(false);

	guiptr->add(hud,"hud");
	guiptr->add(win, "win");
	guiptr->add(lose, "lose");

	createHud();

	loadLevelDatabase();
	getWords();
	rebuildSlate();
	parseDuplication();

	rows[0][0]->setFocused(true);
}

Game::~Game()
{
	sounds_ptr->playtrack(SoundManager::AMBIENTMUSIC);
	hud->removeAllWidgets();
	win->removeAllWidgets();
	lose->removeAllWidgets();
	guiptr->remove(hud);
	guiptr->remove(win);
	guiptr->remove(lose);
}


void Game::update()
{
	for (int i = 0; i < 5; i++)
	{
		if (!rows[y_axis][i]->isFocused())
			continue;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
		{
			if (rows[y_axis][i]->getCaretPosition() < 1 && i > 0)
			{
				rows[y_axis][i]->setFocused(false);
				rows[y_axis][i - 1]->setFocused(true);
				break;

			}

		}

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && keyignoretimer.getElapsedTime().asMilliseconds() > 250.f)
	{
		if (won)
		{
			nextLevel();
			keyignoretimer.restart();
			return;
		}

		if (checkWord())
		{
			won = true;
			sounds_ptr->playtrack(SoundManager::WINMUSIC);
			hud->setVisible(false);
			win->setVisible(true);
			auto levelcounter = win->get<tgui::Label>("Label1");

			std::stringstream ss;
			ss << level;
			levelcounter->setText(ss.str());
			auto star1 = win->get<tgui::Picture>("STAR 1");
			auto star2 = win->get<tgui::Picture>("STAR 2");
			auto star3 = win->get<tgui::Picture>("STAR 3");

			if (y_axis == 0)
			{
				star1->setVisible(true);
				star2->setVisible(true);
				star3->setVisible(true);
				stars = 3;
			}
			else if (y_axis == 1 || y_axis == 2)
			{
				star1->setVisible(true);
				star2->setVisible(true);
				stars = 2;
			}
			else if(y_axis == 3)
			{
				star1->setVisible(true);
				stars = 1;
			}
			saveDatabase();
		}
		else if(y_axis < 4)
		{
			if (guess.size() == 5)
			{
				std::cout << "HELLO\n";
				enableRow(y_axis, false);
				y_axis++;
				enableRow(y_axis);
				rows[y_axis][0]->setFocused(true);
				sounds_ptr->playtrack(SoundManager::LOSESOUND);
			}
		}
		else
		{
			hud->setVisible(false);
			lose->setVisible(true);
			sounds_ptr->playtrack(SoundManager::LOSESCREENSOUND);
			saveDatabase();
		}
		keyignoretimer.restart();
	}
}

void Game::render(sf::RenderTarget& target)
{

}

bool Game::isGameRunning()
{
	bool r = running;
	return r;
}

void Game::createHud()
{
	try
	{
		hud->loadWidgetsFromFile("game.txt");
		textures[0] = tgui::Texture("res/EDIT BOX RED.png");
		textures[1] = tgui::Texture("res/EDIT BOX GREEN.png");
		textures[2] = tgui::Texture("res/EDIT BOX YELLOW.png");
	}
	catch (tgui::Exception e)
	{
		std::cout << e.what() << '\n';
	}

	auto back = hud->get<tgui::Button>("BACK BUTTON");
	back->onClick([&]() {
		running = false; 
		rows.clear();
		sounds_ptr->playtrack(SoundManager::DELETESOUND);

		saveDatabase();
	});

	try 
	{
		win->loadWidgetsFromFile("win.txt");
	}
	catch (tgui::Exception &e)
	{
		std::cout << e.what() << '\n';
	}

	auto next = win->get<tgui::Button>("NEXT BUTTON");

	next->onClick([&]()
	{
		nextLevel();
	});

	auto home = win->get<tgui::Button>("MAIN MENU BUTTON");

	home->onClick([&]()
	{
		running = false;
		sounds_ptr->playtrack(SoundManager::DELETESOUND);

		auto star1 = win->get<tgui::Picture>("STAR 1");
		auto star2 = win->get<tgui::Picture>("STAR 2");
		auto star3 = win->get<tgui::Picture>("STAR 3");

		star1->setVisible(false);
		star2->setVisible(false);
		star3->setVisible(false);


		level++;
		stars = levelDatabase["stars"][level] == NULL ? 0 : levelDatabase["stars"][level].asInt();
		saveDatabase();

	});

	try 
	{
		lose->loadWidgetsFromFile("lose.txt");
	}
	catch (tgui::Exception& e)
	{
		std::cout << e.what() << '\n';
	}

	auto lhome = lose->get<tgui::Button>("MAIN MENU BUTTON");
	lhome->onClick([&]()
	{
		sounds_ptr->playtrack(SoundManager::DELETESOUND);
		running = false;
	});

	auto restart = lose->get<tgui::Button>("RESTART BUTTON");
	restart->onClick([&]()
	{
		level--;
		sounds_ptr->playtrack(SoundManager::DELETESOUND);
		sounds_ptr->playtrack(SoundManager::AMBIENTMUSIC);

		hud->setVisible(true);
		lose->setVisible(false);

		clearSlate();
		rebuildSlate();
	});
}

void Game::createRow(int y)
{
	rows.push_back(std::vector<tgui::EditBox::Ptr>());
	auto editbox = hud->get<tgui::EditBox>("EditBox1");
	
	for (int i = 0; i < 5; i++)
	{
		rows.back().push_back(tgui::EditBox::create());
		rows.back().back() = tgui::EditBox::copy(editbox);
		rows.back().back()->setPosition(editbox->getPosition().x + (i * 200), editbox->getPosition().y + (y * 190));
		rows.back().back()->setMaximumCharacters(2);
		rows.back().back()->setTextSize(0);
		rows.back().back()->setAlignment(tgui::EditBox::Alignment::Center);
		rows.back().back()->getRenderer()->setTextColorDisabled(tgui::Color::Black);
		rows.back().back()->onTextChange([&](tgui::EditBox::Ptr editbox,int index,int Y)
			{
				if (editbox->getCaretPosition() == 1 && index < 4)
				{
					editbox->setFocused(false);
					rows[Y][index + 1]->setFocused(true);
				}
				if (editbox->getCaretPosition() > 1)
				{
					char a = editbox->getText().toStdString()[1];
					editbox->setText(editbox->getText().toStdString()[0]);
					
					std::cout << "MULTIPLE LETTERS\n";
					if (index > 3)
						return;

					rows[Y][index + 1]->setText(a);
				}
				if (editbox->getCaretPosition() < 1)
				{
					sounds_ptr->playtrack(SoundManager::Sound::DELETESOUND);
				}
				if (editbox->getCaretPosition() == 1)
				{
					sounds_ptr->playtrack(SoundManager::Sound::KEYPRESSSOUND);
				}

				
			},rows.back().back(),i,y);
		hud->add(rows.back().back());
		std::cout << rows.back().back()->getMaximumCharacters() << '\n';
	}
}

void Game::enableRow(int y, bool enable)
{
	for (int i = 0; i < 5; i++)
	{
		rows[y][i]->setEnabled(enable);
	}
}

bool Game::checkWord()
{
	guess = std::string();
	for (int i = 0; i < 5; i++)
	{
		if(rows[y_axis][i]->getText().toStdString().size() != 0)
			guess += tolower(rows[y_axis][i]->getText().toStdString()[0]);

		colors[i] = textures[0];
	}
	std::cout <<"GUESS:" << guess << '\n';
	std::cout << "Word Is:" << word << '\n';

	if (guess.size() < 5)
		return false;

	for (int i = 0; i < 5; i++)
	{
		if (word[i] == guess[i])
		{
			std::cout << "Correct\n";
			wordDuplication[guess[i]]--;
			colors[i] = textures[1];
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (word.find(guess[i], 0) != std::string::npos && colors[i] != textures[1])
		{
			if (wordDuplication[guess[i]] > 0)
			{
				colors[i] = textures[2];
				wordDuplication[guess[i]]--;
			}
		}
	}

	std::cout << wordDuplication;

	for (int i = 0; i < 5; i++)
	{
		rows[y_axis][i]->getRenderer()->setTexture(colors[i]);
	}

	if (word == guess)
	{
		std::cout << "YAY!!!\n";
		return true;
	}

	parseDuplication();
	return false;
}

void Game::clearSlate()
{
	//this will do more in the future
	for (int y = 0; y < y_axis; y++)
	{
		for (int i = 0; i < 5; i++)
		{
			guiptr->remove(rows[y][i]);
		}
		rows[y].clear();
	}

	hud->remove(label);
	rows.clear();
}

void Game::rebuildSlate()
{
	selectWord();
	parseDuplication();

	auto label = hud->get<tgui::Label>("Label1");
	level++;
	std::stringstream ss;
	ss << level;
	label->setText(ss.str());

	try
	{
		picture = hud->get<tgui::Picture>("Picture");
	//	sf::Texture texture;
		//texture.loadFromFile(imgpath);
		picture->getRenderer()->setTexture(imgpath.c_str());
	}
	catch (tgui::Exception e)
	{
		std::cout << e.what() << '\n';
	}

	for (int i = 0; i < 5; i++)
	{
		createRow(i);

		if (i > 0)
			enableRow(i, false);
		else
			enableRow(i);
	}

	y_axis = 0;

	rows[0][0]->setFocused(true);
	stars = 0;
}

void Game::getWords()
{
	std::fstream fs;
	fs.open("data/words1.txt");
	
	if (fs.is_open())
	{
		std::string temp;

		while (std::getline(fs, temp))
		{
			words.push_back(utils::decrypt(temp));
		}
	}
	else
	{
		std::cout << "Failed to open Words file\n";
		return;
	}
	fs.close();

	fs.open("data/words-images.json");

	if (fs.is_open())
	{
		std::string temp;
		std::string buffer;
		while (std::getline(fs, temp))
		{
			buffer += utils::decrypt(temp);
		}

		Json::Reader reader;
		reader.parse(buffer, pathDatabase);
		std::cout << pathDatabase << '\n';
	}
	else
	{
		std::cout << "Failed to load json file\n";
	}
	fs.close();
}

void Game::selectWord()
{
	word = words[level];
	std::cout << word << '\n';

	for (int i = 0; i < pathDatabase.size();i++)
	{
		if (pathDatabase[i]["id"].asString() == word)
		{
			imgpath = pathDatabase[i]["path"].asString();
		}
	}

}

void Game::saveDatabase()
{
	std::fstream fs;
	fs.open("data/level.json", std::ios::out);

	if (levelDatabase["currentlevel"].asInt() <= level)
		levelDatabase["currentlevel"] = level;

	if(levelDatabase["stars"][level - 1].asInt() <= stars)
		levelDatabase["stars"][level-1] = stars;

	if(fs.is_open())
	{
		std::string buffer;

		Json::StyledWriter writer;
		buffer = writer.write(levelDatabase);

		fs << utils::encrypt(buffer);
	}

	fs.close();
}

void Game::loadLevelDatabase()
{
	std::fstream fs;
	fs.open("data/level.json");

	if(fs.is_open())
	{
		std::string buffer;
		std::string tmp;

		while (std::getline(fs, tmp))
		{
			buffer += utils::decrypt(tmp);
		}

		Json::Reader reader;
		reader.parse(buffer,levelDatabase);
	}

	fs.close();

	std::cout << levelDatabase << '\n';
}

char Game::tolower(char a)
{
	if (a < 97)
		return a + 32;

	return a;
}

void Game::nextLevel()
{
	if (level >= levelDatabase["maxlevel"].asInt())
	{
		running = false;
		return;
	}

	sounds_ptr->playtrack(SoundManager::AMBIENTMUSIC);
	sounds_ptr->playtrack(SoundManager::DELETESOUND);
	won = false;
	hud->setVisible(true);
	win->setVisible(false);

	clearSlate();
	rebuildSlate();

	auto star1 = win->get<tgui::Picture>("STAR 1");
	auto star2 = win->get<tgui::Picture>("STAR 2");
	auto star3 = win->get<tgui::Picture>("STAR 3");

	star1->setVisible(false);
	star2->setVisible(false);
	star3->setVisible(false);
}

void Game::parseDuplication()
{
	wordDuplication.clear();
	for (int i = 0; i < 5; i++)
	{
		if (wordDuplication.find(word[i]) == wordDuplication.end())
		{
			wordDuplication.insert({ word[i],1 });
		}
		else
		{
			wordDuplication[word[i]] = wordDuplication[word[i]] + 1;
			std::cout << "Word already exists\n";
			std::cout << "Iteration: " << i << '\n';
		}
	}

	std::cout << "First: " << wordDuplication << '\n';
}
