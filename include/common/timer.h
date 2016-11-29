// Name:	timer.h
// Date:    11/05/2016
// Version: 1.0.0.0

#pragma once
#include "macro.h"
#include <ctime>

SYSTEM BEGIN
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
END
