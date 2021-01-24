#include "SumFinder.hpp"

#define SLOWDOWN 0
#define SLOWDOWN_TIME 100

SumFinder SumFinder::currentInstance;	// define this static member (ONCE!)

// private methods
void SumFinder::_slowdown(ul currentIdx) {
	if ((currentIdx % 100) == 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(this->getSlowdownTime()));
	}
}

/* Brute force algorithm */
ull SumFinder::_getSumNT(ul lowerLimit, ul upperLimit) {
	ull total = 0;
	BasicTimer timer;
	timer.start();
	for (ul currNum = lowerLimit; currNum <= upperLimit; ++currNum) {
		if (this->getSlowdownTime() > 0) { this->slowdown(currNum); }
		total += currNum;
	}
	timer.end();
	double duration = timer.getDuration();

	std::cout << "[  LOG  ] getSumNT(): " << duration << "ms" << std::endl;
	return total;
}

/* Partial Sum (threading) algorithm */
void SumFinder::_getPartialSum(ul lowerLimit, ul upperLimit, std::promise<ull>&& prms) { // move expects rvalue ref: &&
	ull total = 0;
	BasicTimer timer;
	timer.start();
	for (ul currNum = lowerLimit; currNum <= upperLimit; ++currNum) {
		if (this->getSlowdownTime() > 0) { this->slowdown(currNum); }
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

ull SumFinder::_getSumT(ul lowerLimit, ul upperLimit, int threadCount) {
	ull total = 0;
	BasicTimer timer;
	timer.start();
	std::vector<std::thread> partialSumthreads(threadCount);
	std::vector<std::promise<ull>> partialSumPrms(threadCount);
	std::vector<std::future<ull>> partialSumFtr(threadCount);
	std::vector<ull> partialSums(threadCount);

	ul incrementValue = upperLimit / threadCount;
	if (upperLimit % threadCount) { ++incrementValue; }	// making sure we overflow upperLimit

	ul currentNumber = lowerLimit;
	for (int threadIdx = 0; threadIdx < threadCount; ++threadIdx) {
		partialSumFtr.at(threadIdx) = partialSumPrms.at(threadIdx).get_future();
		ul upperNumber = currentNumber + incrementValue;
		if (upperNumber > upperLimit) { upperNumber = upperLimit; }
		std::thread thrd(
			&SumFinder::_getPartialSum, 
			this,
			currentNumber, 
			upperNumber, 
			std::move(partialSumPrms.at(threadIdx))
		);
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

/* Partial Sum (async) algorithm: same as 'SumFinder::_getSumNT()' */
ull SumFinder::_getPartialSumAsync(ul lowerLimit, ul upperLimit) { // move expects rvalue ref: &&
	ull total = 0;
	for (ul currNum = lowerLimit; currNum <= upperLimit; ++currNum) {
		if (this->getSlowdownTime() > 0) { this->slowdown(currNum); }
		total += currNum;
	}
	return total;
}

ull SumFinder::_getSumAsync(ul lowerLimit, ul upperLimit, int threadCount) {
	ull total = 0;
	BasicTimer timer;
	timer.start();
	std::vector<std::future<ull>> partialSumFtr(threadCount);
	std::vector<ull> partialSums(threadCount);

	ul incrementValue = upperLimit / threadCount;
	if (upperLimit % threadCount) { ++incrementValue; }	// making sure we overflow upperLimit

	ul currentNumber = lowerLimit;
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

// public methods
SumFinder& SumFinder::init() {
	return currentInstance;
}

void SumFinder::setSlowdownTime(int slowdownTime) {
	init().slowdownTime = slowdownTime;
}

int SumFinder::getSlowdownTime() {
	return init().slowdownTime;
}

void SumFinder::slowdown(ul currentIdx) { 
	init()._slowdown(currentIdx); 
}

/* Brute force algorithm */
ull SumFinder::getSumNT(ul lowerLimit, ul upperLimit) {
	return init()._getSumNT(lowerLimit, upperLimit);
}

/* Partial Sum (threading) algorithm */
void SumFinder::getPartialSum(ul lowerLimit, ul upperLimit, std::promise<ull>&& prms) {
	return init()._getPartialSum(lowerLimit, upperLimit, (std::promise<ull> &&) prms);
}

ull SumFinder::getSumT(ul lowerLimit, ul upperLimit, int threadCount) {
	return init()._getSumT(lowerLimit, upperLimit, threadCount);
}

/* Partial Sum (async) algorithm */
ull SumFinder::getPartialSumAsync(ul lowerLimit, ul upperLimit) {	// move expects rvalue ref: &&
	return init()._getPartialSumAsync(lowerLimit, upperLimit);
}

ull SumFinder::getSumAsync(ul lowerLimit, ul upperLimit, int threadCount) {
	return init()._getSumAsync(lowerLimit, upperLimit, threadCount);
}
