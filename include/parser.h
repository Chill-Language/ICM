#ifndef _ICM_PARSER_H_
#define _ICM_PARSER_H_

#include "ast.h"
#include "match.h"
#include "keyword.h"

namespace ICM
{
	namespace Parser
	{
		AST* createAST(Match &match);
		ObjectPtr createObject(DefaultType type, const string &str);
	}
}

#endif
