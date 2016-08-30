#ifndef _ICM_TOSTRING_
#define _ICM_TOSTRING_

#include "basic.h"
#include "ast.h"
#include "match.h"
#include "function.h"
#include "keyword.h"

namespace ICM
{
	std::string to_string(DefaultType type);
	std::string to_string(KeywordID key);
	std::string to_string(const ObjectPtr &obj);
	std::string to_string(const DataList &obj);
	std::string to_string(const AST *ast);
	std::string to_string(const MatchResult *mr);
	std::string to_string(const ICM::Function::Signature &sign);
}

#endif
