/*
 * EngineSoundThread.h
 *
 *  Created on: Aug 13, 2019
 *      Author: thepursuer
 */

#ifndef ENGINESOUNDTHREAD_H_
#define ENGINESOUNDTHREAD_H_

#include <atomic>

class EngineSoundThread {
public:
	EngineSoundThread(int buzzerPin, const int& maxEngineRPM);
	~EngineSoundThread();

	void run();

	void updateSpeed(int speed);

private:
	volatile std::atomic<int> speed_;
	const int buzzerPin_;
	const int maxEngineRPM_;
};

#endif /* ENGINESOUNDTHREAD_H_ */
