#include <iostream>
#include <string>
#include <fstream>

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <SFML/Graphics.hpp>

std::string removeQuotes(std::string str)
{
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '"')
		{
			str.erase(str.begin() + i);
		}
	}
	return str;
}

std::string getFilename(std::string str)
{
	std::string buffer;
	for (int i = str.size() - 1; i > 0; i--)
	{
		if (str[i] == '/')
			return buffer;

		buffer.insert(buffer.begin(), str[i]);
	}
	return buffer;
}

std::string encrypt(std::string input)
{
	int multiple = 1;
	std::string buffer;
	for (int i = 0; i < input.size(); i++)
	{
		multiple *= -1;
		buffer += input[i] + (1 * multiple);
	}

	return buffer;
}

std::string decrypt(std::string input)
{
	int multiple = 1;
	std::string buffer;
	for (int i = 0; i < input.size(); i++)
	{
		multiple *= -1;
		buffer += input[i] - (1 * multiple);
	}

	return buffer;
}


int main()
{
	while (true)
	{
		std::cout << "What do you want to do\n"
					 "1. Encrypt\n"
					 "2. Decrypt\n";

		int action = 0;
		std::cin >> action;

		std::string str;
		//std::cout << "Please state the location of the file using relative or absolute path:\n";
		//std::cin.ignore();
		//std::getline(std::cin, str);

		std::ifstream ifs;
		std::ofstream ofs;

		std::string paths[] = {
			"C:/Users/dante/OneDrive/Documents/C++/VS Project/Word Guesser/Word Guesser/data/level.json",
			"C:/Users/dante/OneDrive/Documents/C++/VS Project/Word Guesser/Word Guesser/data/words1.txt",
			"C:/Users/dante/OneDrive/Documents/C++/VS Project/Word Guesser/Word Guesser/data/words-images.json"
		};


		for (int i = 0; i < 3; i++)
		{
			ifs.open(paths[i]);
			if (ifs.is_open())
			{
				ofs.open("data/" + getFilename(paths[i]));
				std::string buffer;

				if (action == 1)
				{
					while (std::getline(ifs, buffer))
					{
						ofs << encrypt(buffer) << '\n';
					}
					std::cout << "FILE COMPLETED\n";
				}
				else if(action == 2)
				{
					while (std::getline(ifs, buffer))
					{
						ofs << decrypt(buffer) << '\n';
					}
					std::cout << "FILE COMPLETED\n";
				}
				ofs.close();
			}
			else
			{
				std::cout << "Could not open file:" << str << '\n';
			}
			ifs.close();
		}

	}
}
/*
int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Crypt");
	sf::Event ev;

	tgui::Gui gui;
	gui.setTarget(window);

	try {
		auto filedialog = tgui::FileDialog();//::create();
		//filedialog->onEscapeKeyPress([&]() {std::cout << filedialog->getPath().asString() << '\n'; });
		//gui.add(filedialog);
	}
	catch (tgui::Exception& e)
	{
		std::cout << e.what() << '\n';
	}


	while (window.isOpen())
	{
		while (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				window.close();

			gui.handleEvent(ev);
		}

		window.clear();
		gui.draw();
		window.display();
	}
}
*/