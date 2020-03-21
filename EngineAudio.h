/*
 * EngineAudio.h
 *
 *  Created on: Sep 18, 2019
 *      Author: thepursuer
 */

#ifndef ENGINEAUDIO_H_
#define ENGINEAUDIO_H_

#include <fmod_studio.hpp>

class EngineAudio {
public:
	EngineAudio();
	~EngineAudio();

	void start();
	void stop();
	void updateRPM(int rpm);
	void updateLoad(float load);

	void setVolume(int vol);

	const int maxRPM() const {return 7500;}
private:
	FMOD::Studio::System * sys_;
	FMOD::Studio::Bank * bank_;
	FMOD::Studio::Bank * mbank_;
	FMOD::Studio::EventDescription * eventDes_;
	FMOD::Studio::EventInstance * eventInstance_;
};

#endif /* ENGINEAUDIO_H_ */
