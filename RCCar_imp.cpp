/*
 * RCCar_imp.cpp
 *
 *  Created on: Mar 21, 2020
 *      Author: thepursuer
 */

#include <pca9685.h>
#include <wiringPi.h>

#include "RCCar_imp.h"
#include "RC_Utilities.h"
#include "RC_Settings.h"

RCCarImp::RCCarImp(std::shared_ptr<EngineAudio> audio):
	load_()
{
	//audio.start();
	//audio.setVolume(5);
	digitalWrite(L298N_HBRIDGE_PIN1, LOW);
	digitalWrite(L298N_HBRIDGE_PIN2, HIGH);
}

void RCCarImp::update(){
	//Update steering
	pwmWrite(SERVO_PIN, calcTicks(servoPw_));

	//Update Direction
	if(direction_ == FORWARD){
		digitalWrite(L298N_HBRIDGE_PIN1, LOW);
		digitalWrite(L298N_HBRIDGE_PIN2, HIGH);
	}
	else{
		digitalWrite(L298N_HBRIDGE_PIN1, HIGH);
		digitalWrite(L298N_HBRIDGE_PIN2, LOW);
	}

	//Write the speed out to the mdb
	pwmWrite(L298N_EN_PIN, throttle_);
}
