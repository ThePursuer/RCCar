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
#include <sys/stat.h>
#include <linux/joystick.h>
#include <fcntl.h>

#include "RCCar.h"
#include "RCController.h"
#include "Tachometer.h"
#include "EngineAudio.h"
#include "RC_Settings.h"
#include "RC_Utilities.h"

using namespace std;

//PCA9685 related
int pca9685FD;

//Joystick related
int joystickfd = -1;

//Tachometer related
std::function<void(void)> tachometerCallback;
void tachometerCallback_wrapper(){
	//We cannot pass the Tachomter function directly to wiring pi because of "ruuuules, shhhlerp <pushes up his glasses>"..,
	//so we will wrap it in this function and pass the wrapper insted
	//Screw the rules, I have money!
	tachometerCallback();
}

//Sounds related
EngineAudio audio;

//Utility functions
int calcTicks(float impulseMs){
	//calculate the ticks needed to produce a pulse of impulseMS milliseconds on the pca9685
	float cycleMs = 1000.0f / HERTZ;
	return (int)(MAX_PWM * impulseMs / cycleMs + 0.5f);
}

int joystickFileExists(const char* filename){
	//Check if the file exists
    struct stat buffer;
    int exist = stat(filename,&buffer);
    if(exist == 0)
        return 1;
    else // -1
        return 0;
}

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

//Implement an RC Car
class MyRCCar: public RC_Car{
public:
	MyRCCar(shared_ptr<Tachometer> tach):
		tach_(tach),
		load_(),
		realSpeed_(),
		engineSpeed_(),
		speedToWrite_(),
		previousGear_()
		{
			audio.start();
			audio.setVolume(1);
		}

	void updateRealSpeed(int rpm){ realSpeed_ = number_map<int, int>(rpm, 0, MAX_REAL_SPEED, 0, MAX_PWM); }

protected:
	void update(){
		//Update speed from tachometer
		//updateRealSpeed(tach_->getRPM());

		//Update steering
		pwmWrite(SERVO_PIN, calcTicks(servoPw_));

		//Downshift if the rpms are too low
		if(engineSpeed_ < IDLE_RPM && gearBox.getGear() > 0)
			gearBox.gearDown();

		//Readjust RPMS if we changed gear
		if(previousGear_ != gearBox.getGear() && gearBox.getGear() != 0)
			engineSpeed_ = number_map<int, int>(realSpeed_, gearBox.getGearMin(), gearBox.getGearMax(), IDLE_RPM, MAX_PWM);

		//Update Engine speed and load
		load_ = number_map<int, float>(throttle_, 0, MAX_PWM, -1.0, 1.0);
		engineSpeed_ = engineSpeed_ + (load_ * RPM_DELTA_CLAMP_VALUE);
		engineSpeed_ = clamp<int>(engineSpeed_, 0, MAX_PWM);

		//Get the speed we should write out to the motor drive board
		if(gearBox.getGear() != 0){
			speedToWrite_ = number_map<int, int>(engineSpeed_, 0, MAX_PWM, gearBox.getGearMin(), gearBox.getGearMax());
			speedToWrite_ = speedToWrite_ - (FULL_BRAKE_DELTA * brakeForce_);//Apply brake force here rather than to the engine
		}
		else
			speedToWrite_ = 0;

		realSpeed_ = speedToWrite_;//todo: remove this when we have the tach connected

		//Write the speed out to the mdb
		if(gearBox.getGear() == 0){
			pwmWrite(L298N_EN_PIN, 0);
			pwmWrite(PIN_BASE + 12, 0);
		}
		else if(gearBox.getGear() > 0){
			pwmWrite(L298N_EN_PIN, speedToWrite_);
			pwmWrite(PIN_BASE + 12, 0);
		}
		else{
			pwmWrite(PIN_BASE + 12, speedToWrite_);
			pwmWrite(L298N_EN_PIN, 0);
		}


		//Update the audio
		audio.updateLoad(load_);
		audio.updateRPM(max(engineSpeed_, IDLE_RPM));
		previousGear_ = gearBox.getGear();
	}
private:
	shared_ptr<Tachometer> tach_;

	int load_;
	int engineSpeed_;
	int realSpeed_;
	int speedToWrite_;

	int previousGear_;

	EngineAudio audio;
};

int main(int argc, char **argv) {

	/***************************
	 * Setup the pca9685 first *
	 ***************************/

	// Calling wiringPi setup first.
	wiringPiSetupPhys();

	pinMode(L298N_HBRIDGE1_PIN, OUTPUT);
	pinMode(L298N_HBRIDGE2_PIN, OUTPUT);


	digitalWrite(L298N_HBRIDGE1_PIN, HIGH);
	digitalWrite(L298N_HBRIDGE2_PIN, HIGH);
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

	/************************************
	 * Set up the RC_Car and Controller *
	 ************************************/

	//Start with the tachometer
	shared_ptr<Tachometer> tach = make_shared<Tachometer>(1000);
	pinMode(TACHOMETER_PIN, INPUT);
	//Set the callback and pass the wrapper function
	tachometerCallback = bind(&Tachometer::pulse, tach);
	wiringPiISR(TACHOMETER_PIN, INT_EDGE_FALLING, tachometerCallback_wrapper);

	shared_ptr<MyRCCar> myCar = make_shared<MyRCCar>(tach);
	myCar->setUpdateCycle(50);
	myCar->engineOn();

	RC_Controller controller(myCar);

	/***********************
	 * Start the main loop *
	 ***********************/

	struct js_event jsEvent;
	while(read(joystickfd, &jsEvent, sizeof(jsEvent))){
			if(!joystickFileExists(JOYSTICK_FILENAME)){//If we loose the connection to the controller STOP THE CAR!!!
				myCar->stop();
				openJoystickFile();
			}
			else
				controller.handleJoystickEvent(jsEvent);
	}

	/***********
	 * Cleanup *
	 ***********/

	myCar->stop();
	close(joystickfd);
	return 0;
}
