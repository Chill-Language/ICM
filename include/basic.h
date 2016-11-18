#ifndef _ICM_BASIC_H_
#define _ICM_BASIC_H_

// Includes
//  default
#include <string>
#include <cctype>
#include <memory>
#include <vector>
#include <stack>
#include <map>
#include <functional>
#include <algorithm>
#include <initializer_list>
#include <cassert>
//  common
#include "file.h"
#include "prints.h"
#include "memory.h"
#include "charptr.h"
#include "range.h"
#include "lightlist.h"
#include "bijectionmap.h"

// Declares

// Using
using std::string;
using std::vector;
using std::map;
using std::stack;
using std::shared_ptr;
using std::unique_ptr;
using std::to_string;
using namespace Common;

using byte = uint8_t;
using word = uint16_t;
using dword = uint32_t;
using qword = uint64_t;

template <typename T> constexpr T MaxValue() { return std::numeric_limits<T>::max(); }

namespace ICM
{
#if _ITERATOR_DEBUG_LEVEL != 0
	template <typename T>
	class pointer_iterator : public std::iterator<std::random_access_iterator_tag, T, ptrdiff_t, T*, T&>
	{
	public:
		using _Unchecked_type = iterator;

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

	std::string to_string();
}

namespace ICM
{
	// Output
	template <typename T>
	inline void print(const T &obj)
	{
		using ICM::to_string;
		using Convert::to_string;
		using std::to_string;

		Common::Output::print(to_string(obj));
	}
	template <>
	inline void print(const std::string &obj)
	{
		Common::Output::print(obj);
	}
	inline void print(const char *obj)
	{
		Common::Output::print(obj);
	}
	inline void println()
	{
		Common::Output::println();
	}
	template <typename T>
	inline void println(const T &obj)
	{
		print(obj);
		println();
	}
	template <typename First, typename... Rest>
	inline void print(const First& first, const Rest&... rest)
	{
		print(first);
		print(rest...);
	}
	template <typename First, typename... Rest>
	inline void println(const First& first, const Rest&... rest)
	{
		print(first);
		println(rest...);
	}
}

#endif

#undef BEGIN
#undef END
