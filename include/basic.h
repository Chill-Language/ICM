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
//  common
#include "file.h"
#include "prints.h"
#include "memory.h"
#include "charptr.h"
#include "range.h"
#include "lightlist.h"

// Declares

// Using
using std::string;
using std::vector;
using std::map;
using std::stack;
using std::shared_ptr;
using std::to_string;
using namespace Common;

namespace ICM
{
	// Type
	enum DefaultType {
		BEGIN_TYPE_ENUM, // Begin Enum Mark
		T_Null,
		T_LBracket,
		T_RBracket,
		T_LSBracket,
		T_RSBracket,
		T_Comment,
		T_Object,     // Obj
		T_Nil,        // Nil
		T_Vary,       // Var
		T_Error,      // Err
		T_Identifier, // Idt
		T_Keyword,    // Key
		T_Argument,   // Arg *
		T_List,       // L
		T_Disperse,   // D   *
		T_Number,     // N
		T_String,     // S
		T_Char,       // C
		T_Function,   // F
		T_Type,       // T
		T_Boolean,    // Bool
		T_Symbol,     // Sym
		END_TYPE_ENUM, // End Enum Mark
	};

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
