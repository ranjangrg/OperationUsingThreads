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
	
	SumFinder(int slowdownTimeInMillisec) : slowdownTime(slowdownTimeInMillisec) {};
	SumFinder() : SumFinder(0) {};

	/* Natural sequence: +1 */
	/* Brute force algorithm */
	ull _getNaturalSumNT(ul lowerLimit, ul upperLimit);

	/* Partial Sum (threading) algorithm */
	void _getNaturalPartialSum(ul lowerLimit, ul upperLimit, std::promise<ull>&& prms);

	ull _getNaturalSumT(ul lowerLimit, ul upperLimit, int threadCount);

	/* Partial Sum (async) algorithm */
	ull _getNaturalPartialSumAsync(ul lowerLimit, ul upperLimit);	// move expects rvalue ref: &&

	ull _getNaturalSumAsync(ul lowerLimit, ul upperLimit, int threadCount);

	/* Dataset methods */
	ull _getDatasetSumNT(const std::vector<ul>& dataset);
	void _getDatasetPartialSum(
		const std::vector<ul>& dataset, 
		ul startIdx, ul endIdx, std::promise<ull>&& prms);
	ull _getDatasetSumT(const std::vector<ul>& dataset, int threadCount);

public:
	SumFinder(const SumFinder&) = delete;
	static SumFinder& init();

	void static setSlowdownTime(int slowdownTimeInMillisec);
	int static getSlowdownTime();
	void static sleep();

	ull static add(ull numA, ull numB);	// incl. artificial sleep to simulate heavy load/proccessing

	/* Brute force algorithm */
	ull static getNaturalSumNT(ul lowerLimit, ul upperLimit);

	/* Partial Sum (threading) algorithm */
	void static getNaturalPartialSum(ul lowerLimit, ul upperLimit, std::promise<ull>&& prms);

	ull static getNaturalSumT(ul lowerLimit, ul upperLimit, int threadCount);

	/* Partial Sum (async) algorithm */
	ull static getNaturalPartialSumAsync(ul lowerLimit, ul upperLimit);	// move expects rvalue ref: &&

	ull static getNaturalSumAsync(ul lowerLimit, ul upperLimit, int threadCount);

	ull static getDatasetSumNT(const std::vector<ul>& dataset);

	void static getDatasetPartialSum(
		const std::vector<ul>& dataset, 
		ul startIdx, ul endIdx, std::promise<ull>&& prms);

	ull static getDatasetSumT(const std::vector<ul>& dataset, int threadCount);
};

#endif
