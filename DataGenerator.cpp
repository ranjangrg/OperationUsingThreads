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

std::vector<int> DataGenerator::getDataset(int numCount) {	// C++11 'moves' local vector on return
	std::vector<int> dataset;
	dataset.reserve(numCount);
	for (int idx = 0; idx < numCount; ++idx) {
		dataset.push_back(this->getInt());
	}
	return std::move(dataset);
}
