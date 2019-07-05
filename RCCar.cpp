/*
 * RCCar.cpp
 *
 *  Created on: Jun 21, 2019
 *      Author: thepursuer
 */

#include "RCCar.h"

#include <iostream>
#include <unistd.h>

float RCCAR_CPP_servoMap(int x, int in_min, int in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t RCCAR_CPP_motorMap(int x, int in_min, int in_max, uint16_t out_min, uint16_t out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

RC_Car::RC_Car():
			servoPw_((servoMaxPW + servoMinPW) / 2),
			speed_(0),
			goingForward_(true),
			engineIsOn_(false),
			updateCycle_(20),
			updateThread_(0){
}

RC_Car::~RC_Car() {
	if(engineIsOn_)
		EngineOff();
}

void RC_Car::turn(int16_t val) {
	servoPw_ = RCCAR_CPP_servoMap(val, INT16_MIN, INT16_MAX, servoMinPW, servoMaxPW);
}

void RC_Car::forward(int16_t val) {
	goingForward_ = true;
	speed_ = RCCAR_CPP_motorMap(val, INT16_MIN, INT16_MAX, 0, UINT8_MAX);
}

void RC_Car::backward(int16_t val) {
	goingForward_ = false;
	speed_ = RCCAR_CPP_motorMap(val, INT16_MIN, INT16_MAX, 0, UINT8_MAX);
}

void RC_Car::stop() {
	speed_ = 0;
}

void RC_Car::engineOn() {
	if(!updateThread_){
		engineIsOn_ = true;
		updateThread_ = new std::thread(&RC_Car::loop, this);
	}
}

void RC_Car::EngineOff() {
	speed_ = 0;
	servoPw_ = (servoMaxPW + servoMinPW) / 2;
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

void RC_Car::loop() {
	while(engineIsOn_){
		update();
		usleep(updateCycle_ * 1000);
	}
}

void RC_Car::setServoMinPw(float servoMinPw) {
	servoMinPW = servoMinPw;
}

void RC_Car::setServoMaxPw(float servoMaxPw) {
	servoMaxPW = servoMaxPw;
}



