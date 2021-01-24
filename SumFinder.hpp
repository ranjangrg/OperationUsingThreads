#pragma once

#ifndef __SUM_FINDER_HPP
#define __SUM_FINDER_HPP

#include "Core.hpp"
#include "BasicTimer.hpp"
#include <thread>
#include <future>

class SumFinder {
private:
	static SumFinder currentInstance;

	int slowdownTime;
	
	SumFinder(int slowdownTime) : slowdownTime(slowdownTime) {};
	SumFinder() : SumFinder(0) {};

	void _slowdown(ul currentIdx);

	/* Brute force algorithm */
	ull _getSumNT(ul lowerLimit, ul upperLimit);

	/* Partial Sum (threading) algorithm */
	void _getPartialSum(ul lowerLimit, ul upperLimit, std::promise<ull>&& prms);

	ull _getSumT(ul lowerLimit, ul upperLimit, int threadCount);

	/* Partial Sum (async) algorithm */
	ull _getPartialSumAsync(ul lowerLimit, ul upperLimit);	// move expects rvalue ref: &&

	ull _getSumAsync(ul lowerLimit, ul upperLimit, int threadCount);

public:
	SumFinder(const SumFinder&) = delete;
	static SumFinder& init();

	void static setSlowdownTime(int slowdownTime);
	int static getSlowdownTime();

	void static slowdown(ul currentIdx);

	/* Brute force algorithm */
	ull static getSumNT(ul lowerLimit, ul upperLimit);

	/* Partial Sum (threading) algorithm */
	void static getPartialSum(ul lowerLimit, ul upperLimit, std::promise<ull>&& prms);

	ull static getSumT(ul lowerLimit, ul upperLimit, int threadCount);

	/* Partial Sum (async) algorithm */
	ull static getPartialSumAsync(ul lowerLimit, ul upperLimit);	// move expects rvalue ref: &&

	ull static getSumAsync(ul lowerLimit, ul upperLimit, int threadCount);
};

#endif