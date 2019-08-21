/*
 * Speedometer.cpp
 *
 *  Created on: Aug 14, 2019
 *      Author: thepursuer
 */

#include "Tachometer.h"

#include <unistd.h>

#include <iostream>

using namespace std::chrono;

Tachometer::Tachometer(int MS_timeout):
	startPulse_(high_resolution_clock::now()),
	mstimeout_(MS_timeout),
	pulseDistance_(0)
{

}

void Tachometer::pulse() {
	std::lock_guard<std::mutex> lk (mu_);
	endPulse_ = high_resolution_clock::now();
	pulseDistance_ = duration<double>(endPulse_ - startPulse_).count();
	startPulse_ = endPulse_;
}

int Tachometer::getRPM() {
	std::lock_guard<std::mutex> lk (mu_);
	if(duration<double>(high_resolution_clock::now() - startPulse_).count() * 1000 > mstimeout_)
		return 0;
	if(pulseDistance_ > 0)
		return 60 / pulseDistance_;
	return 0;
}
