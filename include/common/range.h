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
	Range(const T &a, const T &b) : a(a), b(b) {}
	// Data Method
	iterator begin() const {
		return (a > b) ? a + 1 : a;
	}
	iterator end() const {
		return max() + 1;
	}
	bool operator==(const Range &r) const {
		return a == r.a && b == r.b;
	}

	template <typename R> friend std::string to_string(const Range<R> &r);

	// Calculation
	const T& min() const {
		return (a < b) ? a : b;
	}
	const T& max() const {
		return (a > b) ? a : b;
	}
	size_t size() const {
		return max() - min() + 1;
	}

	bool include(T n) const {
		return n >= min() && n <= max();
	}
	bool include(const Range &r) const {
		return min() <= r.min() && max() >= r.max();
	}

	Range reverse() const {
		return Range(b, a);
	}
	Range sequence() const {
		return Range(min(), max());
	}
	Range inverse() const {
		return Range(max(), min());
	}

private:
	T a, b;
};

template <typename T>
Range<T> range(T a, T b)
{
	return Range<T>(a, b);
}

template <typename T>
std::string to_string(const Range<T> &r)
{
	using Convert::to_string;
	using std::to_string;

	return "range(" + to_string(r.a) + "," + to_string(r.b) + ")";
}
END

#endif
