#pragma once

#ifndef __CORE_HPP
#define __CORE_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>

#include <cassert>

typedef unsigned long long ull;
typedef unsigned long ul;

namespace core {
	template <typename T>
	void printVec(std::vector<T>& vect, int digitWidth, int columnCount);
	void printTestResults(std::unordered_map<std::string, bool>& dict);
}

#endif