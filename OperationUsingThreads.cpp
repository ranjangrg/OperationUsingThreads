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
	bool testPassed = true;

	std::vector<double> transactionAmount = {50, -20, -40};
	// Possible closing balance: £10 or £30 or £50 (all 3! possibilities)
	std::vector<double> possibleClosingBalance = {10, 30, 50};

	int threadCount = transactionAmount.size();
	int testCount = 3 * 2;	// to cover all permutation (3! factorial for 3 transactions)

	for (int testIdx = 0; testIdx < testCount && testPassed; ++testIdx) {	
		try {
			BankAccount cust;
			cust.printBalance();

			std::vector<std::thread> threadVec;
			threadVec.reserve(threadCount);

			for (int idx = 0; idx < threadCount; ++idx) {
				double absAmount = std::abs(transactionAmount.at(idx));
				// passsing 'BankAccount' obj as ref because it contains a mutex (non-copyable)
				// as a private class member, BUT thread() 'copies' the agruments and
				// passes it to the callback method/function
				if (transactionAmount.at(idx) > 0) {
					threadVec.emplace_back(&BankAccount::deposit, std::ref(cust), absAmount);
				} else {
					threadVec.emplace_back(&BankAccount::withdraw, std::ref(cust), absAmount);
				}
				//threadVec.at(idx).join();
			}

			for (int idx = 0; idx < threadCount; ++idx) {
				threadVec.at(idx).join();
			}

			double closingBalance = cust.getBalance();
			std::cout << "[ LOG ] Closing Balance: £" << closingBalance << std::endl;

			for (int idx = 0; (idx < possibleClosingBalance.size()) && !testPassed; ++idx) {
				testPassed = possibleClosingBalance.at(idx) == closingBalance;
			}
		} catch(...) {
			testPassed = false;
		}
	}	
	return testPassed;
}

int main() {
	
	std::unordered_map<std::string, bool> testResults;
	testResults["sumFinderNatural"] = testSumFinderNatural();
	testResults["sumFinderDataset"] = testSumFinderDataset();
	testResults["mutexTest"] = testMutex();

	core::printTestResults(testResults);

	std::cout << "--\nmain() successful!\n--\n";
	return 1;
}
