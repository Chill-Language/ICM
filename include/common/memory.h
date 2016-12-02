// Name:	memory.h
// Date:	06/14/2016
// Version:	2.0.0.0

#pragma once
#ifndef _SYSTEM_MEMORY_H_
#define _SYSTEM_MEMORY_H_
#include "macro.h"
#include <cstdlib>
#include <cstring>

SYSTEM BEGIN
#if _ITERATOR_DEBUG_LEVEL != 0
template <typename T>
class pointer_iterator : public std::iterator<std::random_access_iterator_tag, T, ptrdiff_t, T*, T&>
{
public:
	using _Unchecked_type = std::iterator<std::random_access_iterator_tag, T, ptrdiff_t, T*, T&>;

	explicit pointer_iterator(T* p) : p(p) {}
	bool operator!=(const pointer_iterator& i) const {
		return p != i.p;
	}
	bool operator<(const pointer_iterator& i) const {
		return p < i.p;
	}
	T& operator*() const {
		return *p;
	}
	const pointer_iterator& operator++() {
		++p;
		return *this;
	}
	int operator+(const pointer_iterator &i) const {
		return p + i.p;
	}
	int operator-(const pointer_iterator &i) const {
		return p - i.p;
	}
	pointer_iterator operator+(int i) const {
		return pointer_iterator(p + i);
	}
	pointer_iterator operator-(int i) const {
		return pointer_iterator(p - i);
	}

	T* p;
};

template <typename T>
inline pointer_iterator<T> _PointerIterator(T* p) {
	return pointer_iterator<T>(p);
}
#else
template <typename T>
inline T* _PointerIterator(T* p) {
	return p;
}
#endif

using byte = uint8_t;
using word = uint16_t;
using dword = uint32_t;
using qword = uint64_t;

namespace Memory
{
	template <typename T>
	inline size_t get_size(size_t length) {
		return length * (sizeof(T) / sizeof(char));
	}

	// New
	template <typename T>
	inline T* new_(size_t length) {
		return new T[length]();
	}

	template <typename T>
	inline void delete_(T *p) {
		delete[] p;
	}

	template <typename T = byte>
	inline T* alloc_(size_t length) {
		return (T*)std::malloc(length * sizeof(T));
	}
	template <typename T = void>
	inline void free_(T *p) {
		std::free(p);
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
			return copyTo(new_<T>(length), from, length);
	}
}
END

#endif
