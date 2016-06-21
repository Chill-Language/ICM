#ifndef _ICM_TOSTRING_
#define _ICM_TOSTRING_

#include "basic.h"
#include "ast.h"
#include "match.h"

namespace ICM
{
	std::string to_string(DefaultType type);
	std::string to_string(const ObjectPtr &obj);
	std::string to_string(const Function* func);
	std::string to_string(const Parameters *pars);
	std::string to_string(const ASTNode *astn);
	std::string to_string(const AST *ast);
	std::string to_string(const MatchResult *mr);
}

#endif
