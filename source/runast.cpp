#include "basic.h"
#include "runast.h"
#include "objects.h"
#include "tostring.h"

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
		ASTNode *tmp = nullptr;

		switch (node->getNodeType()) {
		case AST_NIL:
			break;
		case AST_DATA:
			result = node;
			break;
		case AST_FUNC:
			auto func = node->getFunc();
			auto pars = node->getPars();
			auto type = func->getType();
			auto id = func->getID();
			auto list = pars->getList();
			auto types = list.front()->getdata()->get_type();
			if (type == FUNC_DEF) {
				// TODO
				switch (id) {
				case 1: // Add
					tmp = new ASTNode(AST_DATA);
					tmp->setdata(Objects::Func::sum(createList(list)));
					result = tmp;
					break;
				case 2: // Sub
					tmp = new ASTNode(AST_DATA);
					tmp->setdata(Objects::Func::sub(createList(list)));
					result = tmp;
					break;
				case 11: // Print
					Objects::Func::print(calcASTNode(list.at(0))->getdata());
					break;
				}
			}
			break;
		}
		return result;
	}

	void runAST(const AST *ast)
	{
		auto a = calcASTNode(ast->getRoot());
		Common::Output::println(a);
	}
}
