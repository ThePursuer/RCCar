/*
 * EngineSoundThread.cpp
 *
 *  Created on: Aug 13, 2019
 *      Author: thepursuer
 */

#include "EngineSoundThread.h"

#include <wiringPi.h>

#include <unistd.h>
#include <iostream>

#define MIN_SLEEP 4000
#define MAX_SLEEP 30000

int ENGINESOUNDTHREAD_CPP_map(int x, int in_max, int in_min)
{
	return (x - in_min) * (MAX_SLEEP - MIN_SLEEP) / (in_max - in_min) + MIN_SLEEP;
}

EngineSoundThread::EngineSoundThread(int buzzerPin, const int& maxEngineRPM):
	buzzerPin_(buzzerPin),
	maxEngineRPM_(maxEngineRPM),
	speed_(MAX_SLEEP)
{
}

EngineSoundThread::~EngineSoundThread() {
}

void EngineSoundThread::run() {
	while(speed_ > 1){
		digitalWrite(buzzerPin_, HIGH);
		usleep(50);
		digitalWrite(buzzerPin_, LOW);
		usleep(speed_ - 50);
	}
}

void EngineSoundThread::updateSpeed(int speed) {
	speed_ = ENGINESOUNDTHREAD_CPP_map(speed, 0, maxEngineRPM_);
}
