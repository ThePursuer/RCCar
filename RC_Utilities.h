/*
 * Utilities.h
 *
 *  Created on: Sep 27, 2019
 *      Author: thepursuer
 */

#ifndef RC_UTILITIES_H_
#define RC_UTILITIES_H_

#include <sys/stat.h>


template<class T1, class T2>
T2 number_map(T1 x, T1 in_min, T1 in_max, T2 out_min, T2 out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template <class T>
T clamp(T val, T minval, T maxval){
	if(val < minval)
		return minval;
	else if( val > maxval)
		return maxval;
	return val;
}

inline int calcTicks(float impulseMs){
	//calculate the ticks needed to produce a pulse of impulseMS milliseconds on the pca9685
	float cycleMs = 1000.0f / HERTZ;
	return (int)(MAX_PWM * impulseMs / cycleMs + 0.5f);
}

inline bool fileExists(const char* filename){
	//Check if the file exists
    struct stat buffer;
    int exist = stat(filename, &buffer);
    if(exist == 0)
        return true;
    else // -1
        return false;
}

#endif /* RC_UTILITIES_H_ */
