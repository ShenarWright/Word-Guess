#include <iostream>
#include <string>
#include <fstream>

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
		std::cout << "Please state the location of the file using relative or absolute path:\n";
		std::cin.ignore();
		std::getline(std::cin, str);

		std::ifstream ifs;
		std::ofstream ofs;

		ifs.open(removeQuotes(str));

		if (ifs.is_open())
		{
			ofs.open("data/file.txt");
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