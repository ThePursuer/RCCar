/*
 * RCCar.cpp
 *
 *  Created on: Jun 21, 2019
 *      Author: thepursuer
 */

#include "RCCar.h"
#include "RC_Utilities.h"

#include <iostream>
#include <unistd.h>


RC_Car::RC_Car():
			servoPw_((servoMaxPW_ + servoMinPW_) / 2),
			throttle_(),
			engineIsOn_(),
			updateCycle_(20),
			updateThread_(),
			direction_(FORWARD){
}

RC_Car::~RC_Car() {
	if(engineIsOn_)
		EngineOff();
}

void RC_Car::turn(int16_t val) {
	std::lock_guard<std::mutex> lk(updateMu_);
	servoPw_ = number_map<int16_t, float>(val, INT16_MIN, INT16_MAX, servoMinPW_, servoMaxPW_);
}

void RC_Car::throttle(int16_t val) {
	std::lock_guard<std::mutex> lk(updateMu_);
	throttle_ = number_map<int16_t, int>(val, INT16_MIN, INT16_MAX, minSpeed_, maxSpeed_);
}

void RC_Car::stop() {
	std::lock_guard<std::mutex> lk(updateMu_);
	throttle_ = 0;
}

void RC_Car::engineOn() {
	if(!updateThread_){
		engineIsOn_ = true;
		updateThread_ = new std::thread(&RC_Car::loop, this);
	}
}

void RC_Car::EngineOff() {
	throttle_ = 0;
	servoPw_ = (servoMaxPW_ + servoMinPW_) / 2;
	engineIsOn_ = false;
	if(updateThread_ && updateThread_->joinable()){
		updateThread_->join();
		delete updateThread_;
	}
	else
		std::cout << "Unable to shut down engine!!" << std::endl;
}

bool RC_Car::setUpdateCycle(uint16_t milliseconds) {
	if(engineIsOn_){
		EngineOff();
		updateCycle_ = milliseconds;
		engineOn();
	}
	else
		updateCycle_ = milliseconds;
	return true;
}

void RC_Car::direction(Direction val) {
	direction_ = val;
}

void RC_Car::loop() {
	while(engineIsOn_){
		std::unique_lock<std::mutex> lk(updateMu_);
		update();
		lk.unlock();
		usleep(updateCycle_ * 1000);
	}
}
