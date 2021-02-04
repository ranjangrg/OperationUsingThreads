#include "Core.hpp"

template <typename T>
void core::printVec(std::vector<T>& vect, int digitWidth, int columnCount) {
	ul dataCount = (ul)(vect.size());
	for (ul idx = 0; idx < dataCount; ++idx) {
		std::cout << std::setw(digitWidth) << vect.at(idx);
		if (((idx + 1) % columnCount) == 0) { std::cout << std::endl; }
	}
	std::cout << std::endl;
}

void core::printTestResults(std::unordered_map<std::string, bool>& dict) {
	std::string result;
	std::cout << "Test Results:" << std::endl;
	for (const auto& currTest: dict) {
		result = currTest.second ? " PASS" : "FAIL";
		std::cout << "[ " << result << " ]: " << currTest.first << std::endl;
	}
}


// explicit instantitation of template methods
// ref: https://en.cppreference.com/w/cpp/language/class_template#Explicit_instantiation
template void core::printVec(std::vector<int>& vect, int digitWidth, int columnCount);
template void core::printVec(std::vector<ul>& vect, int digitWidth, int columnCount);
