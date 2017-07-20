// Name:	lightlist.h
// Date:	08/10/2016
// Version:	1.0.0.0

#pragma once
#ifndef _SYSTEM_LIGHTLIST_H_
#define _SYSTEM_LIGHTLIST_H_
#include "macro.h"
#include "memory.h"

SYSTEM_BEGIN
template <typename T>
class lightlist_creater;

template <typename T>
class lightlist
{
public:
	using iterator = T*;
	using creater = lightlist_creater<T>;
public:
	lightlist() = default;

	explicit lightlist(size_t capacity)
		// This means lightlist can be used for value type (have no destructor).
		: _capacity(capacity), data(Memory::alloc<T>(_capacity), Memory::free<T>) {}

	lightlist(const std::initializer_list<T> &il)
		: lightlist(il.begin(), il.end()) {}

	template <typename Iter>
	lightlist(Iter begin, Iter end)
		: lightlist(static_cast<size_t>(end - begin)) {
		std::copy(begin, end, _PointerIterator(data.get()));
	}

	template <typename Iter>
	lightlist(Iter begin, size_t capacity)
		: lightlist(capacity) {
		std::copy_n(begin, _capacity, _PointerIterator(data.get()));
	}

	template <typename Iter>
	lightlist(Iter begin, Iter end, size_t capacity)
		: lightlist(capacity) {
		std::copy(begin, end, _PointerIterator(data.get()));
	}

	template <size_t N>
	explicit lightlist(T (&arr)[N])
		: lightlist(std::begin(arr), std::end(arr)) {}

	explicit lightlist(const T &t)
		: lightlist({ t }) {}

	~lightlist() {}

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
class lightlist_creater
{
public:
	lightlist_creater(size_t n) : _count(0), _data(n) {}

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
	lightlist<T> data() const {
		lightlist<T> ndata(_data);
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
	lightlist<T> _data;
};
SYSTEM_END

#endif
