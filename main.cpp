/*
 *  Created on: Jun 21, 2019
 *      Author: thepursuer
 */

#include <iostream>
#include <functional>
#include <algorithm>
#include <memory>

#include <pca9685.h>
#include <wiringPi.h>

#include <unistd.h>
#include <linux/joystick.h>
#include <fcntl.h>

#include "RCCar.h"
#include "RCCar_imp.h"
#include "RCController.h"
#include "EngineAudio.h"
#include "RC_Settings.h"
#include "RC_Utilities.h"

using namespace std;

//PCA9685 related
int pca9685FD;

//Joystick related
int joystickfd = -1;

//Sounds related
EngineAudio audio;


void openJoystickFile(){
	//open the joystick file
	joystickfd = open(JOYSTICK_FILENAME, O_RDONLY);
	while(joystickfd < 0)
	{
		ERROR("Could not open joystick file... Retrying");
		joystickfd = open(JOYSTICK_FILENAME, O_RDONLY);

		sleep(2);
	}
}


int main(int argc, char **argv) {
	/***************************
	 * Setup the pca9685 first *
	 ***************************/

	// Calling wiringPi setup first.
	wiringPiSetupPhys();

	pinMode(L298N_HBRIDGE_PIN1, OUTPUT);
	pinMode(L298N_HBRIDGE_PIN2, OUTPUT);


	digitalWrite(L298N_HBRIDGE_PIN1, HIGH);
	digitalWrite(L298N_HBRIDGE_PIN2, HIGH);
	pwmWrite(PIN_BASE + 12, 0);
	pwmWrite(L298N_EN_PIN, 0);

	// Setup with pinbase 300 and i2c location 0x40
	pca9685FD = pca9685Setup(PIN_BASE, 0x40, HERTZ);
	if (pca9685FD < 0)
	{
		ERROR("Error in setup\n");
		return pca9685FD;
	}

	// Reset all output
	pca9685PWMReset(pca9685FD);

	/**************************
	 * Open the Joystick file *
	 **************************/
	openJoystickFile();

	/***************
	 * Audio Setup *
	 ***************/
	std::shared_ptr<EngineAudio> audio = std::make_shared<EngineAudio>();

	/************************************
	 * Set up the RC_Car and Controller *
	 ************************************/
	shared_ptr<RCCarImp> myCar = make_shared<RCCarImp>(audio);
	myCar->setUpdateCycle(50);
	myCar->engineOn();

	RC_Controller controller(myCar);

	/***********************
	 * Start the main loop *
	 ***********************/

	struct js_event jsEvent;
	while(1){
		if(!fileExists(JOYSTICK_FILENAME)){//If we loose the connection to the controller STOP THE CAR!!!
			myCar->stop();
			joystickfd = -1;
			openJoystickFile();
		}
		else{
			read(joystickfd, &jsEvent, sizeof(jsEvent));
			controller.handleJoystickEvent(jsEvent);
		}
	}

	/***********
	 * Cleanup *
	 ***********/

	myCar->stop();
	close(joystickfd);
	return 0;
}
