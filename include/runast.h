#ifndef _ICM_RUNAST_H_
#define _ICM_RUNAST_H_
#include "ast.h"

namespace ICM
{
	const ASTNode* calcASTNode(const ASTNode *node);
	void runAST(const AST *ast);
}

#endif
