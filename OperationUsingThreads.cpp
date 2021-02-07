// OperationUsingThreads.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Core.hpp"
#include "BasicTimer.hpp"
#include "SumFinder.hpp"
#include "DataGenerator.hpp"
#include "BankAccount.hpp"

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

bool testMutex() {
	bool testPassed = false;

	try {
		BankAccount cust;
		cust.printBalance();

		// passsing BankAccount obj as ref because it contains a mutex (non-copyable)
		// as a private class member
		std::thread threadDeposit1(&BankAccount::deposit, std::ref(cust), 100.0);
		std::thread threadWithdraw1(&BankAccount::withdraw, std::ref(cust), 20.0);
		std::thread threadWithdraw2(&BankAccount::withdraw, std::ref(cust), 95.0);
		std::thread threadDeposit2(&BankAccount::deposit, std::ref(cust), 10.0);

		threadDeposit1.join();
		threadWithdraw1.join();
		threadWithdraw2.join();
		threadDeposit2.join();

		/*
		threadDeposit1.detach();
		threadWithdraw1.detach();
		threadWithdraw2.detach();
		threadDeposit2.detach();
		*/

		cust.printBalance();

		testPassed = true;
	} catch(...) {
		testPassed = false;
	}
	
	return testPassed;
}

int main() {
	
	std::unordered_map<std::string, bool> testResults;
	/*
	testResults["sumFinderNatural"] = testSumFinderNatural();
	testResults["sumFinderDataset"] = testSumFinderDataset();
	*/
	testResults["mutexTest"] = testMutex();

	/*
	DataGenerator<ul> dg = DataGenerator<ul>();
	int dataCount = 64;
	std::vector<ul> dataset = dg.getDataset(dataCount);

	core::printVec(dataset, 8, 8);
	*/

	core::printTestResults(testResults);

	std::cout << "--\nmain() successful!\n--\n";
	return 1;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

