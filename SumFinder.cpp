#include "SumFinder.hpp"

#define SLOWDOWN 0
#define SLOWDOWN_TIME 100

#define SLEEP_TIME 500	// in milliseconds

SumFinder SumFinder::currentInstance;	// define this static member (ONCE!)

// private methods
void SumFinder::_slowdown(ul currentIdx) {
	if ((currentIdx % 100) == 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(this->getSlowdownTime()));
	}
}

/* Brute force algorithm */
ull SumFinder::_getNaturalSumNT(ul lowerLimit, ul upperLimit) {
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
void SumFinder::_getNaturalPartialSum(
	ul lowerLimit, ul upperLimit, 
	std::promise<ull>&& prms	// move expects rvalue ref: &&
) {
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

ull SumFinder::_getNaturalSumT(ul lowerLimit, ul upperLimit, int threadCount) {
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
			//&SumFinder::_getNaturalPartialSum,
			//this,	// if function in private e.g. _getNaturalPartialSum()
			SumFinder::getNaturalPartialSum,
			currentNumber, 
			upperNumber, 
			std::move(partialSumPrms.at(threadIdx))
		);
		partialSumthreads.at(threadIdx) = std::move(thrd);
		partialSumthreads.at(threadIdx).detach();
		//partialSums.at(threadIdx) = partialSumFtr.at(threadIdx).get();
		currentNumber = upperNumber + 1;
	}

	for (int threadIdx = 0; threadIdx < threadCount; ++threadIdx) {
		partialSums.at(threadIdx) = partialSumFtr.at(threadIdx).get();
		total += partialSums.at(threadIdx);
	}
	timer.end();
	double duration = timer.getDuration();

	std::cout << "[  LOG  ] getSumT(): " << duration << "ms" << std::endl;

	return total;
}

/* Partial Sum (async) algorithm: same as 'SumFinder::_getSumNT()' */
ull SumFinder::_getNaturalPartialSumAsync(ul lowerLimit, ul upperLimit) { // move expects rvalue ref: &&
	ull total = 0;
	for (ul currNum = lowerLimit; currNum <= upperLimit; ++currNum) {
		if (this->getSlowdownTime() > 0) { this->slowdown(currNum); }
		total += currNum;
	}
	return total;
}

ull SumFinder::_getNaturalSumAsync(ul lowerLimit, ul upperLimit, int threadCount) {
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
		partialSumFtr.at(threadIdx) = std::async(
			std::launch::async,
			&getNaturalPartialSumAsync,
			currentNumber, upperNumber);
		//partialSums.at(threadIdx) = partialSumFtr.at(threadIdx).get();
		currentNumber = upperNumber + 1;
	}

	for (int threadIdx = 0; threadIdx < threadCount; ++threadIdx) {
		partialSums.at(threadIdx) = partialSumFtr.at(threadIdx).get();
		total += partialSums.at(threadIdx);
	}
	timer.end();
	double duration = timer.getDuration();

	std::cout << "[  LOG  ] getSumAsync(): " << duration << "ms" << std::endl;

	return total;
}

ull SumFinder::_getDatasetSumNT(const std::vector<ul>& dataset) {
	ull total = 0;
	BasicTimer timer;
	timer.start();
	for (const ul currNum : dataset) {
		total += currNum;
		// DELETE ME:
		this->sleep(SLEEP_TIME);
	}
	timer.end();
	double duration = timer.getDuration();

	std::cout << "[  LOG  ] getDatasetSumNT(): " << duration << "ms" << std::endl;

	return total;
}

void SumFinder::_getDatasetPartialSum(
	const std::vector<ul>& dataset,
	ul startIdx, ul endIdx,
	std::promise<ull>&& prms
) {
	ull total = 0;
	BasicTimer timer;
	timer.start();
		// partition dataset first
		for (ul currIdx = startIdx; currIdx <= endIdx; ++currIdx) {
			total += dataset.at(currIdx);
			// DELETE ME:
			this->sleep(SLEEP_TIME);
		}
	timer.end();
	double duration = timer.getDuration();
	prms.set_value(total);
}

ull SumFinder::_getDatasetSumT(const std::vector<ul>& dataset, int _threadCount) {
	ull total = 0;
	ul datasetSize = (ul)(dataset.size());
	ul threadCount = (ul)_threadCount;

	BasicTimer timer;
	timer.start();
		std::vector<std::thread> partialSumthreads(threadCount);
		std::vector<std::promise<ull>> partialSumPrms(threadCount);
		std::vector<std::future<ull>> partialSumFtr(threadCount);
		std::vector<ull> partialSums(threadCount);

		ul partitionSize = datasetSize / threadCount;
		if (datasetSize % threadCount) { ++partitionSize; }	// making sure we overflow upperLimit

		ul currentIdx = 0;
		for (ul threadIdx = 0; threadIdx < threadCount; ++threadIdx) {
			partialSumFtr.at(threadIdx) = partialSumPrms.at(threadIdx).get_future();
			ul upperIdx = currentIdx + partitionSize;
			if (upperIdx > datasetSize) { upperIdx = datasetSize - 1; }
			std::thread thrd(
				SumFinder::getDatasetPartialSum,
				dataset,
				currentIdx, upperIdx,
				std::move(partialSumPrms.at(threadIdx))
			);
			partialSumthreads.at(threadIdx) = std::move(thrd);
			partialSumthreads.at(threadIdx).detach();
			//partialSums.at(threadIdx) = partialSumFtr.at(threadIdx).get();
			currentIdx = upperIdx + 1;
		}

		for (ul threadIdx = 0; threadIdx < threadCount; ++threadIdx) {
			partialSums.at(threadIdx) = partialSumFtr.at(threadIdx).get();
			total += partialSums.at(threadIdx);
		}
	timer.end();
	double duration = timer.getDuration();

	std::cout << "[  LOG  ] getDatasetSumT(): " << duration << "ms" << std::endl;

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

void SumFinder::sleep(int milsec) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milsec));
}

/* Brute force algorithm */
ull SumFinder::getNaturalSumNT(ul lowerLimit, ul upperLimit) {
	return init()._getNaturalSumNT(lowerLimit, upperLimit);
}

/* Partial Sum (threading) algorithm */
void SumFinder::getNaturalPartialSum(
	ul lowerLimit, ul upperLimit, 
	std::promise<ull>&& prms
) {
	return init()._getNaturalPartialSum(lowerLimit, upperLimit, (std::promise<ull> &&) prms);
}

ull SumFinder::getNaturalSumT(ul lowerLimit, ul upperLimit, int threadCount) {
	return init()._getNaturalSumT(lowerLimit, upperLimit, threadCount);
}

/* Partial Sum (async) algorithm */
ull SumFinder::getNaturalPartialSumAsync(ul lowerLimit, ul upperLimit) {	// move expects rvalue ref: &&
	return init()._getNaturalPartialSumAsync(lowerLimit, upperLimit);
}

ull SumFinder::getNaturalSumAsync(ul lowerLimit, ul upperLimit, int threadCount) {
	return init()._getNaturalSumAsync(lowerLimit, upperLimit, threadCount);
}

ull SumFinder::getDatasetSumNT(const std::vector<ul>& dataset) {
	return init()._getDatasetSumNT(dataset);
}

void SumFinder::getDatasetPartialSum(
	const std::vector<ul>& dataset, 
	ul startIdx, ul endIdx, 
	std::promise<ull>&& prms
) {
	return init()._getDatasetPartialSum(
		dataset, 
		startIdx, endIdx, (std::promise<ull>&&) prms
	);
}

ull SumFinder::getDatasetSumT( const std::vector<ul>& dataset, int threadCount ) {
	return init()._getDatasetSumT(dataset, threadCount);
}