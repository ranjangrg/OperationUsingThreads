#pragma once
#ifndef __BASIC_TIMER_HPP
#define __BASIC_TIMER_HPP

#include "Core.hpp"
#include <chrono>

class BasicTimer {
public:
	std::chrono::high_resolution_clock::time_point startTime, endTime;
	std::chrono::duration<double, std::milli> duration;
	BasicTimer();
	void start();
	void end();
	double getDuration();
	/* Reference */
	//auto startTime = std::chrono::high_resolution_clock::now();
	//auto endTime = std::chrono::high_resolution_clock::now();
	//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
};

#endif