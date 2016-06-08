#ifndef _ICM_ASTS_H_
#define _ICM_ASTS_H_

#include "ast.h"
#include "match.h"
#include "keyword.h"

namespace ICM
{
	AST* createAST(Match &match, const KeyWordMap &KeyWords);
}

#endif
