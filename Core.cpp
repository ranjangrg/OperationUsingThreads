#include "Core.hpp"

void core::printVec(std::vector<int>& vect, int digitWidth, int columnCount) {
	int dataCount = vect.size();
	for (ul idx = 0; idx < dataCount; ++idx) {
		std::cout << std::setw(digitWidth) << vect.at(idx);
		if (((idx + 1) % columnCount) == 0) { std::cout << std::endl; }
	}
	std::cout << std::endl;
}

void core::printTestResults(std::unordered_map<std::string, bool>& dict) {
	for (const auto& currTest: dict) {
	std::string result = currTest.second ? " PASS" : "FAIL";
		std::cout << "[ " << result << " ]: " << currTest.first << std::endl;
	}
}