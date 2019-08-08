/*
 *  Created on: Jun 21, 2019
 *      Author: thepursuer
 */

#include <iostream>
#include <unistd.h>

#include <pca9685.h>
#include <wiringPi.h>

#include <sys/stat.h>
#include <linux/joystick.h>
#include <fcntl.h>

#include "RCCar.h"
#include "RCController.h"

using namespace std;

//Main related
bool run = true;

//Utility
#define ERROR(msg) do {cout << "ERROR: " << msg << endl;} while(0)
#define INFO(msg) do {cout << "INFO: " << msg << endl;} while(0)

//PCA9685 related
#define PIN_BASE 300
#define SERVO_PIN PIN_BASE
#define MAX_PWM 4096
#define HERTZ 330

#define SERVO_MAX_PW 1.7
#define SERVO_MIN_PW 0.5

int pca9685FD;

//Joystick related
#define JOYSTICK_FILENAME "/dev/input/js0"
int joystickfd = -1;

//Motor related
#define L298N_HBRIDGE1_PIN 38 //GPIO20
#define L298N_HBRIDGE2_PIN 40 //GPIO21
#define L298N_EN_PIN (PIN_BASE + 1)

/*
 * Utility function to calculate the ticks needed to produce a pulse of impulseMS milliseconds on the pca9685
 */
int calcTicks(float impulseMs)
{
	float cycleMs = 1000.0f / HERTZ;
	return (int)(MAX_PWM * impulseMs / cycleMs + 0.5f);
}

/*
 * Implement an RC Car
 */
class MyRCCar: public RC_Car{
public:
	MyRCCar(){
		setServoMaxPw(SERVO_MAX_PW);
		setServoMinPw(SERVO_MIN_PW);

		setMaxSpeed(MAX_PWM);
	}

protected:
	void update(){
		pwmWrite(SERVO_PIN, calcTicks(servoPw_));

		if(goingForward_) {
			digitalWrite(L298N_HBRIDGE1_PIN, HIGH);
			digitalWrite(L298N_HBRIDGE2_PIN, LOW);
		}
		else {
			digitalWrite(L298N_HBRIDGE1_PIN, LOW);
			digitalWrite(L298N_HBRIDGE2_PIN, HIGH);
		}
		pwmWrite(L298N_EN_PIN, speed_);
	}
};

int main(int argc, char **argv) {

	/***************************
	 * Setup the pca9685 first *
	 ***************************/

	// Calling wiringPi setup first.
	wiringPiSetupPhys();

	pinMode(L298N_HBRIDGE1_PIN, OUTPUT);
	pinMode(L298N_HBRIDGE2_PIN, OUTPUT);

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

	joystickfd = open(JOYSTICK_FILENAME, O_RDONLY);
	if(joystickfd < 0)
	{
		ERROR("Could not open joystick file");
		return 1;
	}

	/************************************
	 * Set up the RC_Car and Controller *
	 ************************************/

	shared_ptr<MyRCCar> myCar = make_shared<MyRCCar>();
	myCar->setUpdateCycle(50);
	myCar->engineOn();

	RC_Controller controller(myCar);

	/***********************
	 * Start the main loop *
	 ***********************/

	struct js_event jsEvent;
	while(run){
		while(read(joystickfd, &jsEvent, sizeof(jsEvent)))
			controller.handleJoystickEvent(jsEvent);
	}

	/***********
	 * Cleanup *
	 ***********/

	close(joystickfd);
	return 0;
}


