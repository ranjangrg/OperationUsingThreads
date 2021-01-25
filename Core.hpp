#pragma once

#ifndef __CORE_HPP
#define __CORE_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>

typedef unsigned long long ull;
typedef unsigned long ul;

namespace core {
	void printVec(std::vector<int>& vect, int digitWidth, int columnCount);
	void printTestResults(std::unordered_map<std::string, bool>& dict);
}

#endif