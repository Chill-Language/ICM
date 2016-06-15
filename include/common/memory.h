// Name:	memory.h
// Date:	06/14/2016
// Version:	2.0.0.0

#ifndef _SYSTEM_MEMORY_H_
#define _SYSTEM_MEMORY_H_
#include "macro.h"
#include <cstdlib>
#include <cstring>

SYSTEM BEGIN
namespace Memory
{
	template <typename T>
	inline size_t get_size(size_t length) {
		return length * (sizeof(T) / sizeof(char));
	}

	// New
	template <typename T>
	inline T* create(size_t length) {
		return malloc(length);
	}

	template <typename T>
	inline T* create(size_t length, const T &inc) {
		T* ptr = create<T>(length);
		while (length--) {
			*ptr = inc;
			++ptr;
		}
	}

	template <typename T>
	inline T* clear(T* dat, size_t length) {
		bzero((void*)dat, get_size<T>(length));
		return dat;
	}

	template <typename T>
	inline T* copyTo(T* to, const T* from, size_t length) {
		return (T*)memcpy((void*)to, (void*)from, get_size<T>(length));
	}

	template <typename T>
	inline T* copyOf(const T* from, size_t length) {
		if (from == nullptr)
			return nullptr;
		else
			return copyTo(create<T>(length), from, length);
	}
}
END

#endif
