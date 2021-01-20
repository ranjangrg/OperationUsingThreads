// OperationUsingThreads.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Core.h"
#include "BasicTimer.h"
#include <thread>
#include <chrono>
#include <future>

#define SLOWDOWN 1
#define SLOWDOWN_TIME 100

void slowdown(ul currentIdx) {
	if ((currentIdx % 100) == 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(SLOWDOWN_TIME));
	}
}

/* Brute force algorithm */
ull getSumNT(ul upperLimit) {
	ull total = 0;
	ul lowerLimit = 1;
	BasicTimer timer;
	timer.start();
		for (ul currNum = lowerLimit; currNum <= upperLimit; ++currNum) {
			if (SLOWDOWN) { slowdown(currNum); }
			total += currNum;
		}
	timer.end();
	double duration = timer.getDuration();

	std::cout << "[  LOG  ] getSumNT(): " << duration << "ms" << std::endl;
	return total;
}

/* Partial Sum (threading) algorithm */
void getPartialSum(ul lowerLimit, ul upperLimit, std::promise<ull>&& prms) { // move expects rvalue ref: &&
	ull total = 0;
	BasicTimer timer;
	timer.start();
		for (ul currNum = lowerLimit; currNum <= upperLimit; ++currNum) {
			if (SLOWDOWN) { slowdown(currNum); }
			total += currNum;
		}
	timer.end();
	double duration = timer.getDuration();
	prms.set_value(total);
	/*
	try {
		throw std::exception("Testing exception!");
	}
	catch (std::exception e) {
		prms.set_exception(std::current_exception());
	}
	*/
}

ull getSumT(ul upperLimit, int threadCount) {
	ull total = 0;
	BasicTimer timer;
	timer.start();
		std::vector<std::thread> partialSumthreads(threadCount);
		std::vector<std::promise<ull>> partialSumPrms(threadCount);
		std::vector<std::future<ull>> partialSumFtr(threadCount);
		std::vector<ull> partialSums(threadCount);

		ul incrementValue = upperLimit / threadCount;
		if (upperLimit % threadCount) { ++incrementValue; }	// making sure we overflow upperLimit

		ul currentNumber = 1;
		for (int threadIdx = 0; threadIdx < threadCount; ++threadIdx) {
			partialSumFtr.at(threadIdx) = partialSumPrms.at(threadIdx).get_future();
			ul upperNumber = currentNumber + incrementValue;
			if (upperNumber > upperLimit) { upperNumber = upperLimit; }
			std::thread thrd(getPartialSum, currentNumber, upperNumber, std::move(partialSumPrms.at(threadIdx)));
			partialSumthreads.at(threadIdx) = std::move(thrd);
			partialSumthreads.at(threadIdx).detach();
			partialSums.at(threadIdx) = partialSumFtr.at(threadIdx).get();
			currentNumber = upperNumber + 1;
		}

		for (int threadIdx = 0; threadIdx < threadCount; ++threadIdx) {
			total += partialSums.at(threadIdx);
		}
	timer.end();
	double duration = timer.getDuration();

	std::cout << "[  LOG  ] getSumT(): " << duration << "ms" << std::endl;

	return total;
}

/* Partial Sum (async) algorithm */
ull getPartialSumAsync(ul lowerLimit, ul upperLimit) { // move expects rvalue ref: &&
	ull total = 0;
	for (ul currNum = lowerLimit; currNum <= upperLimit; ++currNum) {
		if (SLOWDOWN) { slowdown(currNum); }
		total += currNum;
	}
	return total;
}

ull getSumAsync(ul upperLimit, int threadCount) {
	ull total = 0;
	BasicTimer timer;
	timer.start();
	std::vector<std::future<ull>> partialSumFtr(threadCount);
	std::vector<ull> partialSums(threadCount);

	ul incrementValue = upperLimit / threadCount;
	if (upperLimit % threadCount) { ++incrementValue; }	// making sure we overflow upperLimit

	ul currentNumber = 1;
	for (int threadIdx = 0; threadIdx < threadCount; ++threadIdx) {
		ul upperNumber = currentNumber + incrementValue;
		if (upperNumber > upperLimit) { upperNumber = upperLimit; }
		partialSumFtr.at(threadIdx) = std::async(&getPartialSumAsync, currentNumber, upperNumber);
		partialSums.at(threadIdx) = partialSumFtr.at(threadIdx).get();
		currentNumber = upperNumber + 1;
	}

	for (int threadIdx = 0; threadIdx < threadCount; ++threadIdx) {
		total += partialSums.at(threadIdx);
	}
	timer.end();
	double duration = timer.getDuration();

	std::cout << "[  LOG  ] getSumAsync(): " << duration << "ms" << std::endl;

	return total;
}

int main() {
	int threadCount = 16;
	//ul upperLimit = 4294967295;
	ul upperLimit = 65536;
	ull totalNT = getSumNT(upperLimit);
	ull totalT = getSumT(upperLimit, threadCount);
	ull totalAsy = getSumAsync(upperLimit, threadCount);

	std::cout << "[  LOG  ] SumNT : " << totalNT << std::endl;
	std::cout << "[  LOG  ] SumT  : " << totalT << std::endl;
	std::cout << "[  LOG  ] SumAs : " << totalAsy << std::endl;

	std::cout << "--\nExecution successful!\n--\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
