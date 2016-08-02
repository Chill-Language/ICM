#include "basic.h"
#include "runast.h"
#include "objects.h"
#include "tostring.h"
#include "keyword.h"
#include "function.h"

namespace ICM
{
	const ASTNode* calcASTNode(const ASTNode *node);

	DataList createList(const std::vector<ASTNode*> &list)
	{
		DataList listnum;
		for (auto &l : list)
			listnum.push_back(calcASTNode(l)->getdata());
		return listnum;
	}
	const ASTNode* calcASTNode(const ASTNode *node)
	{
		using namespace Objects;

		const ASTNode* result = nullptr;

		if (node == nullptr)
			return result;

		switch (node->getNodeType()) {
		case AST_NIL:
			break;
		case AST_DATA:
			result = node;
			break;
		case AST_NODE:
			auto func = node->getFunc();
			auto pars = node->getPars();
			auto type = func->getType();
			auto id = func->getID();
			auto list = pars->getList();
			if (type == FUNC_DEF) {
				ASTNode *tmp = nullptr;
				tmp = new ASTNode(AST_DATA);
				auto &tmpp = checkCall(FuncTable[id], createList(list));
				tmp->setdata(tmpp);
				result = tmp;
			}
			break;
		}
		return result;
	}

	void runAST(const AST *ast)
	{
		using Common::Output::println;
		auto a = calcASTNode(ast->getRoot());
		println();
		println("\nResult:");
		println(a);
	}
}
