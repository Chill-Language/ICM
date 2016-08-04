#ifndef _ICM_RUNAST_H_
#define _ICM_RUNAST_H_
#include "ast.h"

namespace ICM
{
	ASTNode* calcASTNode(const ASTNode *node);
	DataList createList(const std::vector<ASTNode*> &list);
	ASTNode* runAST(const AST *ast);
}

#endif
