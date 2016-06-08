#include "ast.h"
#include "match.h"
#include "keyword.h"

namespace ICM
{
	AST* createAST(Match &match, const KeyWordMap &KeyWords);
}