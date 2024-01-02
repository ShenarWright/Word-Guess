#pragma once

#include "pch.h"

std::ostream& operator<<(std::ostream& os, std::map<char, int> map)
{
	for (auto& e : map)
	{
		os << "[" << e.first << "," << e.second << "]\n";
	}
	return os;
}