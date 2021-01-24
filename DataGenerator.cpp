#include "DataGenerator.hpp"

DataGenerator::DataGenerator(int lowerLimit, int upperLimit) {
	this->dist = std::uniform_int_distribution<int>(lowerLimit, upperLimit);
}

/*
DataGenerator::~DataGenerator() {
	//delete this->generator;
}
*/

int DataGenerator::getInt() {
	//auto val = std::bind(this->dist, this->generator);
	return this->dist(this->generator);
}