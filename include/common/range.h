// Name:	range.h
// Date:	06/14/2016
// Version:	2.0.0.0

#ifndef _SYSTEM_RANGE_H_
#define _SYSTEM_RANGE_H_
#include "macro.h"
#include <string>
#include "prints.h"

SYSTEM BEGIN
template <typename T>
class Range
{
public:
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
public:
	Range(const T &bg, const T &ed) : _begin(bg), _end(ed) {}
	// Data Method
	iterator begin() const {
		return _begin;
	}
	iterator end() const {
		return _end;
	}
	bool operator==(const Range &r) const {
		return _begin == r._begin && _end == r._end;
	}
	T operator[](size_t i) const {
		return _begin + i;
	}

	template <typename R> friend std::string to_string(const Range<R> &r);

	// Calculation
	size_t size() const {
		return _end - _begin;
	}

	bool include(T n) const {
		return n >= _begin && n < _end;
	}
	bool include(const Range &r) const {
		return _begin <= r._begin && _end >= r._end;
	}

private:
	T _begin, _end;
};

template <typename T>
Range<T> range(T a, T b)
{
	return Range<T>(a, b);
}
Range<size_t> range(int a, size_t b);

template <typename T>
std::string to_string(const Range<T> &r)
{
	using Convert::to_string;
	using std::to_string;

	return "range(" + to_string(r.a) + "," + to_string(r.b) + ")";
}
END

#endif
