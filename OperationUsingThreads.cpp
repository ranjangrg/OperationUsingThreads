// OperationUsingThreads.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Core.hpp"
#include "BasicTimer.hpp"
#include "SumFinder.hpp"
#include "DataGenerator.hpp"

bool testSumFinderNatural() {
	bool testPassed = false;
	try {
		int threadCount = 8;
		ul lowerLimit = 1;
		//ul upperLimit = 4294967295;
		ul upperLimit = 100;

		//SumFinder& sumFinderInstance = SumFinder::init();

		SumFinder::setSlowdownTime(10);
		ull totalNaturalNT = SumFinder::getNaturalSumNT(lowerLimit, upperLimit);
		ull totalNaturalT = SumFinder::getNaturalSumT(lowerLimit, upperLimit, threadCount);
		ull totalNaturalAsy = SumFinder::getNaturalSumAsync(lowerLimit, upperLimit, threadCount);

		std::cout << "[  CHK  ] SumNaturalNT : " << totalNaturalNT << std::endl;
		std::cout << "[  CHK  ] SumNaturalT  : " << totalNaturalT << std::endl;
		std::cout << "[  CHK  ] SumNaturalAs : " << totalNaturalAsy << std::endl;
		testPassed = totalNaturalNT == totalNaturalT;
		testPassed = (testPassed) ? (totalNaturalT == totalNaturalAsy) : false;
	}
	catch (...) {
		testPassed = false;
	}
	return testPassed;
}

bool testSumFinderDataset() {
	bool testPassed = false;
	DataGenerator<ul> dg = DataGenerator<ul>();
	//ul dataCount = 4294967295;
	ul dataCount = 100;
	int threadCount = 16;
	std::vector<ul> dataset = dg.getDataset(dataCount);
	//core::printVec(dataset, 8, 8);
	SumFinder::setSlowdownTime(100);
	
	try {
		ull totalDatasetNT = SumFinder::getDatasetSumNT(dataset);
		ull totalDatasetT = SumFinder::getDatasetSumT(dataset, threadCount);

		std::cout << "[  CHK  ] SumDatasetNT : " << totalDatasetNT << std::endl;
		std::cout << "[  CHK  ] SumDatasetT : " << totalDatasetT << std::endl;
		testPassed = totalDatasetNT == totalDatasetT;
	}
	catch (...) {
		testPassed = false;
	}

	return testPassed;
}

int main() {
	std::unordered_map<std::string, bool> testResults;
	testResults["sumFinderNatural"] = testSumFinderNatural();
	testResults["sumFinderDataset"] = testSumFinderDataset();

	/*
	DataGenerator<ul> dg = DataGenerator<ul>();
	int dataCount = 64;
	std::vector<ul> dataset = dg.getDataset(dataCount);

	core::printVec(dataset, 8, 8);
	*/

	core::printTestResults(testResults);

	std::cout << "--\nmain() successful!\n--\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

