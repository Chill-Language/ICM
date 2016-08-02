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
//  common
#include "prints.h"
#include "memory.h"
#include "charptr.h"
#include "range.h"

// Declares

// Using
using std::string;
using std::vector;
using std::map;
using std::to_string;
using namespace Common;

namespace ICM
{
	// Type
	enum DefaultType {
		T_Null,
		T_Object,
		T_Nil,
		T_Vary,
		T_Error,
		T_LBracket,
		T_RBracket,
		T_LSBracket,
		T_RSBracket,
		T_Boolean,
		T_Number,
		T_String,
		T_Symbol,
		T_List,
		T_Comment,
		T_Identifier, // Un Whole Match
	};

	// AutoPtr
	template <typename T> using autoptr = std::shared_ptr<T>;

	std::string to_string();
}

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

#endif
