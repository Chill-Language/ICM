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
#include <list>
#include <functional>
#include <algorithm>
#include <initializer_list>
#include <cassert>
#include <bitset>
#include <utility>
//  common
#include "file.h"
#include "prints.h"
#include "memory.h"
#include "charptr.h"
#include "range.h"
#include "lightlist.h"
#include "dyarray.h"
#include "bijectionmap.h"
#include "indextable.h"

// Declares
using int_t = std::conditional_t<std::is_same<size_t, uint64_t>::value, int64_t, int32_t>;
using uint_t = std::conditional_t<std::is_same<size_t, uint64_t>::value, uint64_t, uint32_t>;
using inth_t = std::conditional_t<std::is_same<size_t, uint64_t>::value, int32_t, int16_t>;
using uinth_t = std::conditional_t<std::is_same<size_t, uint64_t>::value, uint32_t, uint16_t>;

// Using
using std::string;
using std::vector;
using std::map;
using std::stack;
using std::shared_ptr;
using std::unique_ptr;
using std::list;
using std::bitset;
using std::to_string;
using namespace Common;

template <typename T> constexpr T MaxValue() { return std::numeric_limits<T>::max(); }

template <typename _DTy, typename _STy> _DTy& unsafe_any_cast(_STy& data) { return *((_DTy*)(&data)); }


template <typename _Ty, _Ty... _DfV>
class ExplicitType;

template <typename _Ty>
class ExplicitType<_Ty>
{
public:
	explicit ExplicitType(const _Ty &data)
		: data(data) {}

	explicit ExplicitType(_Ty &&data)
		: data(data) {}

	_Ty data;
};

template <typename _Ty, _Ty _DfV>
class ExplicitType<_Ty, _DfV> : public ExplicitType<_Ty>
{
public:
	explicit ExplicitType(const _Ty &data)
		: ExplicitType<_Ty>(data) {}

	explicit ExplicitType(_Ty &&data)
		: ExplicitType<_Ty>(data) {}

	ExplicitType()
		: ExplicitType<_Ty>(_DfV) {}
};


namespace ICM
{
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
