#include "DataGenerator.hpp"

template <typename T>
DataGenerator<T>::DataGenerator(T lowerLimit, T upperLimit) {
	this->dist = std::uniform_int_distribution<T>(lowerLimit, upperLimit);
}

/*
template <typename T>
DataGenerator<T>::~DataGenerator() {
	//delete this->generator;
}
*/

template <typename T>
T DataGenerator<T>::getInt() {
	//auto val = std::bind(this->dist, this->generator);
	return this->dist(this->generator);
}

template <typename T>
std::vector<T> DataGenerator<T>::getDataset(T numCount) {
	std::vector<T> dataset;
	dataset.reserve(numCount);
	for (int idx = 0; idx < numCount; ++idx) {
		dataset.push_back(this->getInt());
	}
	return std::move(dataset);	// C++11 'moves' local vector on return
}

// explicit initialization
// ref: https://en.cppreference.com/w/cpp/language/class_template
template class DataGenerator<int>;
template class DataGenerator<ul>;