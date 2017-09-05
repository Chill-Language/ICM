// Name:	random.h
// Date:    07/07/2017
// Version: 1.0.0.0

#pragma once
#ifndef _SYSTEM_RANDOM_H_
#define _SYSTEM_RANDOM_H_
#include "macro.h"
#include <cstdlib>

SYSTEM_BEGIN
inline int random_base(int n, int (*rand)(void))
{
	int rand_result;
	int rand_max = RAND_MAX / n * n;
	do {
		rand_result = rand();
	} while (rand_result >= rand_max);
	return rand_result % n;
}
inline int random(int n)
{
	return random_base(n, std::rand);
}
inline int random(int begin, int end)
{
	return random(end - begin) + begin;
}
SYSTEM_END

#endif
