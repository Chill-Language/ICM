// Name:	dyarray.h
// Date:	07/20/2017
// Version:	1.0.0.0

#pragma once
#ifndef _SYSTEM_DYARRAY_H_
#define _SYSTEM_DYARRAY_H_
#include "macro.h"
#include "memory.h"
#include <cstdio>

SYSTEM_BEGIN
template <typename T>
class dyarray_creater;

template <typename T>
class dyarray
{
public:
	using iterator = T*;
	using creater = dyarray_creater<T>;
public:
	dyarray() = default;

	explicit dyarray(size_t capacity)
		// This means dyarray can be used for class type (have destructor).
		: _capacity(capacity), data(Memory::new_<T>(_capacity), Memory::delete_<T>) {}

private:
	template <typename Iter>
	explicit dyarray(size_t capacity, Iter copy_iter_begin)
		// This function is used in other constructors.
		: _capacity(capacity), data(Memory::alloc<T>(_capacity), [&](T* p) { Memory::delete_n<T>(p, _capacity); }) {
		Memory::copy_n<T>(data.get(), copy_iter_begin, capacity);
	}
	template <typename Iter>
	explicit dyarray(size_t capacity, Iter copy_iter_begin, size_t create_begin)
		// This function is used in other constructors.
		: _capacity(capacity), data(Memory::alloc<T>(_capacity), [&](T* p) { Memory::delete_n<T>(p, _capacity); }) {
		Memory::copy_n<T>(data.get(), copy_iter_begin, create_begin);
		Memory::new_n<T>(data.get() + create_begin, capacity - create_begin);
	}

public:

	dyarray(const std::initializer_list<T> &il)
		: dyarray(il.begin(), il.end()) {}

	template <typename Iter>
	dyarray(Iter begin, Iter end)
		: dyarray(static_cast<size_t>(end - begin), begin) {
	}

	template <typename Iter>
	dyarray(Iter begin, size_t capacity)
		: dyarray(capacity, begin) {
	}

	template <typename Iter>
	dyarray(Iter begin, Iter end, size_t capacity)  // end - begin < capacity
		: dyarray(capacity, begin, static_cast<size_t>(end - begin)) {
	}

	template <size_t N>
	explicit dyarray(T (&arr)[N])
		: dyarray(std::begin(arr), std::end(arr)) {}

	explicit dyarray(const T &t)
		: dyarray({ t }) {}

	~dyarray() {}

	T* begin() { return data.get(); }
	T* end() { return data.get() + _capacity; }
	const T* begin() const { return data.get(); }
	const T* end() const { return data.get() + _capacity; }
	const T* cbegin() const { return data.get(); }
	const T* cend() const { return data.get() + _capacity; }

	T& operator[](size_t id) { return data.get()[id]; }
	const T& operator[](size_t id) const { return data.get()[id]; }

	T& at(size_t id) { assert(id < size()); return this->operator[](id); }
	const T& at(size_t id) const { assert(id < size()); return this->operator[](id); }

	T& front() { return *begin(); }
	const T& front() const { return *begin(); }
	T& back() { return *(data.get() + _capacity - 1); }
	const T& back() const { return *(data.get() + _capacity - 1); }

	size_t size() const { return _capacity; }
	bool empty() const { return _capacity == 0; }
	void resize(size_t nsize) { _capacity = std::min(_capacity, nsize); }

private:
	size_t _capacity = 0;
	std::shared_ptr<T> data = nullptr;
};

template <typename T>
class dyarray_creater
{
public:
	dyarray_creater(size_t n) : _count(0), _data(n) {}

	void push_back(const T &e) {
		_data[_count++] = e;
	}
	template <typename Iter>
	void insert(Iter beg, Iter end) {
		for (auto &e : rangei(beg, end)) {
			push_back(e);
		}
	}
	void clear() {
		for (size_t i : range(0, _data.size()))
			_data[i].~T();
		_count = 0;
	}
	dyarray<T> data() const {
		dyarray<T> ndata(_data);
		ndata.resize(_count);
		return ndata;
	}
	size_t size() const {
		return _count;
	}
	size_t capacity() const {
		return _data.size();
	}

private:
	size_t _count;
	dyarray<T> _data;
};
SYSTEM_END

#endif
