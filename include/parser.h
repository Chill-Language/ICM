#ifndef _ICM_PARSER_H_
#define _ICM_PARSER_H_

#include "ast.h"
#include "match.h"

namespace ICM
{
	AST* createAST(Match &match, const KeyWordMap &KeyWords);
}

#endif
