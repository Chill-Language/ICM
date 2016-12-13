// Name:	bytepool.h
// Date:	12/06/2016
// Version:	1.0.0.0

#pragma once
#ifndef _SYSTEM_BYTEPOOL_H_
#define _SYSTEM_BYTEPOOL_H_
#include "macro.h"
#include "memory.h"

SYSTEM BEGIN
class BytePool
{
public:
	BytePool() {}
	template <typename T>
	size_t insert(const T &t) {
		size_t size = data.size();
		data.resize(size + sizeof(T));
		Memory::ncopy(dat<T>(size), t);
		return size;
	}
	size_t insert(const void *ptr, size_t len) {
		size_t size = data.size();
		data.resize(size + len);
		Memory::mcopy(get(size), ptr, len);
		return size;
	}
	template <typename T = void>
	T* get(size_t index) {
		return reinterpret_cast<T*>(data.data() + index);
	}
	template <typename T>
	T& dat(size_t index) {
		return *get<T>(index);
	}
	size_t index() const {
		return data.size();
	}
	void clear() {
		data.clear();
	}

private:
	vector<byte> data;
};

template <typename _VTy>
struct ByteSet
{
	union {
		_VTy value;
		byte bytes[sizeof(_VTy)];
	};

	ByteSet& operator=(_VTy val) {
		value = val;
		return *this;
	}
	operator _VTy() const {
		return value;
	}

	byte& operator()(int index) {
		return const_cast<byte&>(const_cast<const ByteSet*>(this)->operator()(index));
	}
	const byte& operator()(int index) const {
		assert(index < (int)sizeof(_VTy));
		assert(index + (int)sizeof(_VTy) >= 0);
		return bytes[(index + (int)sizeof(_VTy)) % (int)sizeof(_VTy)];
	}
};
END

#endif
