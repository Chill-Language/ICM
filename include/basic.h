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
//  common
#include "prints.h"
#include "memory.h"
#include "charptr.h"

// Declares

// Using
using std::string;
using std::to_string;
using namespace Common;
using namespace Output;

namespace ICM
{
	// Type
	enum DefaultType {
		T_Null,
		T_Nil,
		T_LBracket,
		T_RBracket,
		T_Boolean,
		T_Number,
		T_String,
		T_Comment,
		T_Identifier, // Un Whole Match
	};

	// KeyWord
	using KeyWordMap = std::map<std::string, int>;
	using KeyWord = KeyWordMap::value_type;

	// AutoPtr
	template <typename T> using autoptr = std::shared_ptr<T>;
}

#endif
