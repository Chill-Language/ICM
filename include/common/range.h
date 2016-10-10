// Name:	range.h
// Date:	06/14/2016
// Version:	2.0.0.0

#ifndef _SYSTEM_RANGE_H_
#define _SYSTEM_RANGE_H_
#include "macro.h"
#include <string>
#include "prints.h"

SYSTEM BEGIN
template <typename T, typename Iter>
class RangeBase
{
public:
	using type = T;
	using iterator = Iter;
public:
	RangeBase(const T &bg, const T &ed) : _begin(bg), _end(ed) {}
	// Data Method
	Iter begin() const {
		return _begin;
	}
	Iter end() const {
		return _end;
	}
	bool operator==(const RangeBase &r) const {
		return _begin == r._begin && _end == r._end;
	}

	// Calculation
	size_t size() const {
		return _end - _begin;
	}

	bool include(const T &n) const {
		return n >= _begin && n < _end;
	}
	bool include(const RangeBase &r) const {
		return _begin <= r._begin && _end >= r._end;
	}

	template <typename R, typename F> friend std::string to_string(const RangeBase<R, F> &r);

protected:
	T _begin, _end;
};

namespace
{
	template <typename T>
	class iterator
	{
	public:
		iterator(T value) : value(value) {}
		operator T() const {
			return value;
		}
		T operator*() const {
			return value;
		}
		bool operator!=(const iterator& i) const {
			return value != i.value;
		}
		const iterator& operator++() {
			++value;
			return *this;
		}

	private:
		T value;
	};
}

template <typename T>
class Range : public RangeBase<T, iterator<T>>
{
public:
	Range(const T &bg, const T &ed) : RangeBase(bg, ed) {}

	T operator[](size_t i) const {
		return _begin + i;
	}
};

template <typename T>
class RangeIterator : public RangeBase<T, T>
{
public:
	RangeIterator(const T &bg, const T &ed) : RangeBase(bg, ed) {}

	auto operator[](size_t i) const {
		return *(_begin + i);
	}
};

template <typename T>
Range<T> range(const T &a, const T &b)
{
	return Range<T>(a, b);
}
Range<size_t> range(int a, size_t b);

template <typename T>
RangeIterator<T> rangei(T a, T b)
{
	return RangeIterator<T>(a, b);
}

template <typename T1, typename T2>
std::string to_string(const RangeBase<T1, T2> &r)
{
	using Convert::to_string;
	using std::to_string;

	return "range(" + to_string(r._begin) + "," + to_string(r._end) + ")";
}
END

#endif
