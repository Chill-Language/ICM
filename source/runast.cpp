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
			if (type == FUNC_DEF) {
				// TODO
				switch (id) {
				case 1: // Add
					tmp = new ASTNode(AST_DATA);
					tmp->setdata(Objects::Func::add(createList(list)));
					result = tmp;
					break;
				case 2: // Sub
					tmp = new ASTNode(AST_DATA);
					tmp->setdata(Objects::Func::sub(createList(list)));
					result = tmp;
					break;
				case 3: // Mul
					tmp = new ASTNode(AST_DATA);
					tmp->setdata(Objects::Func::mul(createList(list)));
					result = tmp;
					break;
				case 4: // Div
					tmp = new ASTNode(AST_DATA);
					tmp->setdata(Objects::Func::div(createList(list)));
					result = tmp;
					break;
				case 5: // Mod
					tmp = new ASTNode(AST_DATA);
					tmp->setdata(Objects::Func::mod(createList(list)));
					result = tmp;
					break;
				case 21: // Print
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
		Common::Output::println();
		Common::Output::println(a);
	}
}
