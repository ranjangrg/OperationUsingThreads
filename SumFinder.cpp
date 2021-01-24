#include "SumFinder.hpp"

#define SLOWDOWN 0
#define SLOWDOWN_TIME 100

SumFinder SumFinder::currentInstance;	// define this static member (ONCE!)

void SumFinder::_slowdown(ul currentIdx) {
	if ((currentIdx % 100) == 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(SLOWDOWN_TIME));
	}
}

/* Brute force algorithm */
ull SumFinder::_getSumNT(ul lowerLimit, ul upperLimit) {
	ull total = 0;
	return total;
}

/* Partial Sum (threading) algorithm */
void SumFinder::_getPartialSum(ul lowerLimit, ul upperLimit, std::promise<ull>&& prms) { // move expects rvalue ref: &&
	ull total = 0;
	BasicTimer timer;
	timer.start();
	for (ul currNum = lowerLimit; currNum <= upperLimit; ++currNum) {
		if (SLOWDOWN) { _slowdown(currNum); }
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
	return total;
}

/* Partial Sum (async) algorithm */
ull SumFinder::_getPartialSumAsync(ul lowerLimit, ul upperLimit) { // move expects rvalue ref: &&
	ull total = 0;
	return total;
}

ull SumFinder::_getSumAsync(ul lowerLimit, ul upperLimit, int threadCount) {
	ull total = 0;
	return total;
}

// public methods
SumFinder& SumFinder::init() {
	return currentInstance;
}

void SumFinder::slowdown(ul currentIdx) { 
	std::cout << "Slowdown here\n";
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
