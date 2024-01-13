#include "Utils.h"

std::string utils::encrypt(std::string input)
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

std::string utils::decrypt(std::string input)
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

std::string utils::loadEntireFile(std::string path)
{
	std::fstream fs;
	std::string buffer, temp;

	fs.open(path);
	if (fs.is_open())
	{
		while (std::getline(fs, temp))
		{
			buffer += temp;
		}
	}
	else
	{
		std::cout << "Failed to open file\n";
	}
	fs.close();
	return buffer;
}

bool utils::saveFile(std::string path,std::string buffer)
{
	std::fstream fs;
	fs.open(path,std::ios::out);

	if (fs.is_open())
	{
		fs << buffer;
		fs.close();
		return true;
	}
	else
	{
		std::cout << "Failed to open file\n";
	}
	return false;
}
