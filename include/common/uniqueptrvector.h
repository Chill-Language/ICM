// Name:	uniqueptrvector.h
// Date:	08/12/2017
// Version:	1.0.0.0

#pragma once
#ifndef _SYSTEM_UNIQUEPTRVECTOR_H_
#define _SYSTEM_UNIQUEPTRVECTOR_H_
#include "macro.h"
#include <vector>

SYSTEM_BEGIN
template <typename T, typename Allocator = std::allocator<T*>>
class UniquePtrVector
{
	using vectype = std::vector<T*, Allocator>;
public:
	template <typename _VTy>
	class iterator_base {
	public:
		iterator_base(_VTy *data, size_t count = 0)
			: data(data), count(count) {}

		using value_type = typename std::conditional<std::is_const<_VTy>::value, const T, T>::type;

		auto& operator*() { return get(); }
		auto* operator->() { return &get(); }
		auto& operator*() const { return get(); }
		auto* operator->() const { return &get(); }
		auto& operator[](std::ptrdiff_t diff) { return get(diff); }
		const auto& operator[](std::ptrdiff_t diff) const { return get(diff); }
		iterator_base& operator++() {
			++count;
			return *this;
		}
		iterator_base& operator--() {
			--count;
			return *this;
		}
		iterator_base operator+(std::ptrdiff_t diff) {
			return iterator_base(data, count + diff);
		}
		iterator_base operator-(std::ptrdiff_t diff) {
			return iterator_base(data, count - diff);
		}
		bool operator==(const iterator_base &iter) const {
			return data == iter.data && count == iter.count;
		}
		bool operator!=(const iterator_base &iter) const {
			return data != iter.data || count != iter.count;
		}
		bool operator>(const iterator_base &iter) const {
			return count > iter.count;
		}
		bool operator>=(const iterator_base &iter) const {
			return count >= iter.count;
		}
		bool operator<(const iterator_base &iter) const {
			return count < iter.count;
		}
		bool operator<=(const iterator_base &iter) const {
			return count <= iter.count;
		}

	private:
		_VTy *data;
		size_t count;
		value_type& get(std::ptrdiff_t diff = 0) {
			return *(*data)[count + diff];
		}
		const value_type& get(std::ptrdiff_t diff = 0) const {
			return *(*data)[count + diff];
		}
	};
	using iterator = iterator_base<vectype>;
	using const_iterator = iterator_base<const vectype>;

public:
	UniquePtrVector() {}

	explicit UniquePtrVector(size_t length) : data(length) {
		for (size_t i = 0; i != length; ++i) {
			data.push_back(new T());
		}
	}

	~UniquePtrVector() {
		for (T* p : data) {
			if (p) {
				delete p;
			}
		}
	}

	template <typename... Args>
	void push_back(Args... args) {
		data.push_back(new T(args...));
	}
	void push_back(T &&t) {
		data.push_back(new T(t));
	}
	void push_back(const T &t) {
		data.push_back(new T(t));
	}

	void pop_back() {
		delete data.back();
		data.pop_back();
	}

	T& operator[](size_t n) { return *data[n]; }
	const T& operator[](size_t n) const { return *data[n]; }
	T& at(size_t n) { return *data.at(n); }
	const T& at(size_t n) const { return *data.at(n); }
	T* get(size_t n) { return data[n]; }
	const T* get(size_t n) const { return data[n]; }

	iterator begin() { return iterator(&data); }
	iterator end() { return iterator(&data, data.size()); }
	const_iterator begin() const { return const_iterator(&data); }
	const_iterator end() const { return const_iterator(&data, data.size()); }
	const_iterator cbegin() const { return const_iterator(&data); }
	const_iterator cend() const { return const_iterator(&data, data.size()); }
	const_iterator cbegin() { return const_iterator(&data); }
	const_iterator cend() { return const_iterator(&data, data.size()); }

private:
	std::vector<T*> data;
};
SYSTEM_END

#endif
