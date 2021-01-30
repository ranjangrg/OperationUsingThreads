#pragma once

#ifndef __DATA_GENERATOR_HPP
#define __DATA_GENERATOR_HPP

#include "Core.hpp"
#include <random>
//#include <functional>	// only for std::bind

#define LOWER_LIMIT 0
#define UPPER_LIMIT 50000

// generally type 'T' is int OR ul
template <typename T>
class DataGenerator {
private:
	std::default_random_engine generator;
	std::uniform_int_distribution<T> dist;
public:
	DataGenerator(T lowerLimit, T upperLimit);
	DataGenerator() : DataGenerator(LOWER_LIMIT, UPPER_LIMIT) {};
	//~DataGenerator();
	T getNext();
	std::vector<T> getDataset(T numCount);
};

#endif