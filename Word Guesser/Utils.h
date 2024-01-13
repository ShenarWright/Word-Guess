#pragma once
#include "pch.h"

namespace utils
{
	std::string encrypt(std::string input);

	std::string decrypt(std::string input);

	std::string loadEntireFile(std::string path);

	bool saveFile(std::string path,std::string buffer);

}
