// OperationUsingThreads.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Core.hpp"
#include "BasicTimer.hpp"
#include "SumFinder.hpp"
#include "DataGenerator.hpp"

int main() {
	int threadCount = 8;
	ul lowerLimit = 1;
	//ul upperLimit = 4294967295;
	ul upperLimit = 100;

	//SumFinder& sumFinderInstance = SumFinder::init();

	SumFinder::setSlowdownTime(64);
	ull totalNT = SumFinder::getSumNT(lowerLimit, upperLimit);
	ull totalT = SumFinder::getSumT(lowerLimit, upperLimit, threadCount);
	ull totalAsy = SumFinder::getSumAsync(lowerLimit, upperLimit, threadCount);

	std::cout << "[  CHK  ] SumNT : " << totalNT << std::endl;
	std::cout << "[  CHK  ] SumT  : " << totalT << std::endl;
	std::cout << "[  CHK  ] SumAs : " << totalAsy << std::endl;

	DataGenerator dg = DataGenerator();

	for (ul idx = 0; idx < 32; ++idx) {
		std::cout << dg.getInt() << std::endl;
	}

	std::cout << "--\nmain() successful!\n--\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
