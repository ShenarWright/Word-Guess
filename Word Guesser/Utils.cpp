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
