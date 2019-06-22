/*
 * RCCar.cpp
 *
 *  Created on: Jun 21, 2019
 *      Author: thepursuer
 */

#include "RCCar.h"

#include <iostream>
#include <unistd.h>

float RCCAR_CPP_map(int x, int in_min, int in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

RCCar::RCCar():
			servoPw_(1.5),
			speed_(0),
			goingForward_(true),
			engineIsOn_(false),
			updateCycle_(20),
			updateThread_(0){
}

RCCar::~RCCar() {
	if(engineIsOn_)
		EngineOff();
}

void RCCar::turn(int16_t val) {
	servoPw_ = RCCAR_CPP_map(val, INT16_MIN, INT16_MAX, servoMinPW, servoMaxPW);
}

void RCCar::forward(uint8_t val) {
	goingForward_ = true;
	if(val > 100)
		val = 100;
	speed_ = val;
}

void RCCar::backward(uint8_t val) {
	goingForward_ = false;
	if(val > 100)
		val = 100;
	speed_ = val;
}

void RCCar::stop() {
	speed_ = 0;
}

void RCCar::engineOn() {
	if(!updateThread_){
		engineIsOn_ = true;
		updateThread_ = new std::thread(&RCCar::loop, this);
	}
}

void RCCar::EngineOff() {
	speed_ = 0;
	servoPw_ = 1.5;
	engineIsOn_ = false;
	if(updateThread_ && updateThread_->joinable()){
		updateThread_->join();
		delete updateThread_;
	}
	else
		std::cout << "Unable to shut down engine!!" << std::endl;
}

bool RCCar::setUpdateCycle(uint16_t milliseconds) {
	if(engineIsOn_){
		EngineOff();
		updateCycle_ = milliseconds;
		engineOn();
	}
	else
		updateCycle_ = milliseconds;
	return true;
}

void RCCar::loop() {
	while(engineIsOn_){
		update();
		usleep(updateCycle_ * 1000);
	}
}

void RCCar::setServoMinPw(float servoMinPw) {
	servoMinPW = servoMinPw;
}

void RCCar::setServoMaxPw(float servoMaxPw) {
	servoMaxPW = servoMaxPw;
}



