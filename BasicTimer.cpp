#include "BasicTimer.h"

BasicTimer::BasicTimer() {
	this->duration = std::chrono::milliseconds::zero();
};

void BasicTimer::start() {
	this->startTime = std::chrono::high_resolution_clock::now();
};
	
void BasicTimer::end() {
	this->endTime = std::chrono::high_resolution_clock::now();
	this->duration = std::chrono::duration_cast<std::chrono::milliseconds>(this->endTime - this->startTime);
};

double BasicTimer::getDuration() {
	return this->duration.count();
};