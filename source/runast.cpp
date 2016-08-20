#include "basic.h"
#include "runast.h"
#include "objects.h"
#include "tostring.h"
#include "keyword.h"
#include "function.h"

namespace ICM
{
	ASTNode* calcASTNode(const ASTNode *node)
	{
		if (node == nullptr)
			return nullptr;

		switch (node->getNodeType()) {
		case AST_NIL:
			return nullptr;
		case AST_DATA:
			return const_cast<ASTNode*>(node);
		case AST_NODE: {
			auto func = node->getFunc();
			if (func->getType() == FUNC_DEF) {
				auto id = func->getID();
				auto &list = node->getPars()->getList();
				ASTNode* result = new ASTNode(AST_DATA);
				ObjectPtr data = checkCall(DefFuncTable[id], createList(list));
				result->setdata(data);
				return result;
			}
			else if (func->getType() == FUNC_ADD){
				auto id = func->getID();
				auto &list = node->getPars()->getList();
				ASTNode* result = new ASTNode(AST_DATA);
				ObjectPtr data = checkCall(AddFuncTable[id], createList(list));
				result->setdata(data);
				return result;
			}
		}
		default:
			return nullptr;
		}
	}
	DataList createList(const std::vector<ASTNode*> &list)
	{
		DataList listnum;
		for (auto &l : list)
			listnum.push_back(calcASTNode(l)->getdata());
		return listnum;
	}
	ASTNode* runAST(const AST *ast)
	{
		using Common::Output::println;
		return calcASTNode(ast->getRoot());
	}
}
