// Name:	timer.h
// Date:    11/05/2016
// Version: 1.0.0.0

#pragma once
#ifndef _SYSTEM_TIMER_H_
#define _SYSTEM_TIMER_H_
#include "macro.h"
#include <ctime>

SYSTEM_BEGIN
class Timer
{
public:
	Timer() : time(clock()) {}
	size_t detTime() {
		clock_t currTime = clock();
		size_t det = (size_t)(currTime - time);
		time = currTime;
		return det;
	}

	clock_t time;
};
SYSTEM_END

#endif
