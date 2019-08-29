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

int RCCAR_CPP_motorMap(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

RC_Car::RC_Car():
			servoPw_((servoMaxPW_ + servoMinPW_) / 2),
			targetSpeed_(0),
			goingForward_(true),
			gear_(0),
			engineIsOn_(false),
			updateCycle_(20),
			updateThread_(0){
}

RC_Car::~RC_Car() {
	if(engineIsOn_)
		EngineOff();
}

void RC_Car::turn(int16_t val) {
	std::lock_guard<std::mutex> lk(updateMu_);
	servoPw_ = RCCAR_CPP_servoMap(val, INT16_MIN, INT16_MAX, servoMinPW_, servoMaxPW_);
}

void RC_Car::forward(int16_t val) {
	std::lock_guard<std::mutex> lk(updateMu_);
	goingForward_ = true;
	targetSpeed_ = RCCAR_CPP_motorMap(val, INT16_MIN, INT16_MAX, minSpeed_, maxSpeed_);
}

void RC_Car::backward(int16_t val) {
	std::lock_guard<std::mutex> lk(updateMu_);
	goingForward_ = false;
	targetSpeed_ = RCCAR_CPP_motorMap(val, INT16_MIN, INT16_MAX, minSpeed_, maxSpeed_);
}

void RC_Car::stop() {
	std::lock_guard<std::mutex> lk(updateMu_);
	targetSpeed_ = 0;
}

void RC_Car::engineOn() {
	if(!updateThread_){
		engineIsOn_ = true;
		updateThread_ = new std::thread(&RC_Car::loop, this);
	}
}

void RC_Car::EngineOff() {
	targetSpeed_ = 0;
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

void RC_Car::setMaxSpeed(int max_speed) {
	maxSpeed_ = max_speed;
}

void RC_Car::setMinSpeed(int min_speed) {
	minSpeed_ = min_speed;
}

void RC_Car::gearUp() {
	std::lock_guard<std::mutex> lk(updateMu_);
	if(gear_ < 6)
		gear_++;
}

void RC_Car::gearDown() {
	std::lock_guard<std::mutex> lk(updateMu_);
	if(gear_ > -1)
		gear_--;
}

void RC_Car::loop() {
	while(engineIsOn_){
		std::unique_lock<std::mutex> lk(updateMu_);
		update();
		lk.unlock();
		usleep(updateCycle_ * 1000);
	}
}

void RC_Car::setServoMinPw(float servoMinPw) {
	servoMinPW_ = servoMinPw;
}

void RC_Car::setServoMaxPw(float servoMaxPw) {
	servoMaxPW_ = servoMaxPw;
}



