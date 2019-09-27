/*
 * EngineAudio.cpp
 *
 *  Created on: Sep 18, 2019
 *      Author: thepursuer
 */

#include "EngineAudio.h"

#include <fmod_errors.h>
#include <iostream>

#define MIATABANK "Vehicles.bank"
#define MASTERBANK "master.bank"

#define PRINTERR(err) do{ \
		if(err != FMOD_OK) \
			std::cout << __FILE__ << ":" << __LINE__ << ": " << FMOD_ErrorString(err) << std::endl; \
	}while(0)

EngineAudio::EngineAudio() {
	//Initialize System
	auto res = FMOD::Studio::System::create(&sys_);
	PRINTERR(res);
	res = sys_->initialize(512, FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE, FMOD_INIT_NORMAL, 0);
	PRINTERR(res);

	//Debug
	//res = FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_WARNING | FMOD_DEBUG_DISPLAY_LINENUMBERS | FMOD_DEBUG_TYPE_CODEC);
	//PRINTERR(res);

	//Load banks
	res = sys_->loadBankFile(MIATABANK, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank_);
	PRINTERR(res);
	res = sys_->loadBankFile(MASTERBANK, FMOD_STUDIO_LOAD_BANK_NORMAL, &mbank_);
	PRINTERR(res);

	//Get Events
	int count = 0, written = 0;
	res = bank_->getEventCount(&count);
	PRINTERR(res);
	res = bank_->getEventList(&eventDes_, count, &written);
	PRINTERR(res);

	//CreateEventInstances
	res = eventDes_->createInstance(&eventInstance_);
	PRINTERR(res);
	sys_->update();
}

EngineAudio::~EngineAudio() {
	stop();
	auto res = sys_->release();
	PRINTERR(res);
	*bank_;
}

void EngineAudio::start() {
	auto res = eventInstance_->start();
	PRINTERR(res);
	sys_->update();
}

void EngineAudio::stop() {
	auto res = eventInstance_->stop(FMOD_STUDIO_STOP_IMMEDIATE);
	PRINTERR(res);
	sys_->update();
}

void EngineAudio::updateRPM(int rpm) {
	auto res = eventInstance_->setParameterByName("rpm", rpm);
	PRINTERR(res);
	sys_->update();
}

void EngineAudio::updateLoad(float load) {
	auto res = eventInstance_->setParameterByName("load", load);
	PRINTERR(res);
	sys_->update();
}

void EngineAudio::setVolume(int vol) {
	auto res = eventInstance_->setVolume(vol);
	PRINTERR(res);
	sys_->update();
}
