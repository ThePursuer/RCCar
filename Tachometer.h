/*
 * Speedometer.h
 *
 *  Created on: Aug 14, 2019
 *      Author: thepursuer
 */

#ifndef TACHOMETER_H_
#define TACHOMETER_H_

#define MAX_SAMPLES 3

#include <chrono>
#include <mutex>

/*
 * Tachometer Class: A simple tachometer that counts rpm by time between pulse() calls. The counter is reset after time is elapsed.
 */
class Tachometer {
public:
	//MS_timeout is the time in milliseconds it should take before the rpm is reset to zero.
	Tachometer(int MS_timeout);
	Tachometer() = delete;

	void pulse();
	int getRPM();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::high_resolution_clock::duration> startPulse_;
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::high_resolution_clock::duration> endPulse_;
	double pulseDistance_;

	std::mutex mu_;
	int mstimeout_;
};

#endif /* TACHOMETER_H_ */
