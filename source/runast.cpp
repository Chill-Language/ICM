#include "runast.h"
#include "objects.h"
#include "tostring.h"
#include "prints.h"

namespace ICM
{

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
			auto types = list.front()->getdata<Object>().get_type();
			if (type == FUNC_DEF) {
				// TODO
				switch (id) {
				case 1: // Add
					if (types == T_Number) {
						DataList<Number> listnum;
						for (auto &l : list) {
							auto tl = calcASTNode(l);
							if (tl->getObjtype() == T_Number)
								listnum.push_back(tl->getdata<Number>());
						}
						tmp = new ASTNode(AST_DATA);
						tmp->setdata(Objects::sum(listnum));
						result = tmp;
					}
					else if (types == T_String) {
						DataList<String> liststr;
						for (auto &l : list) {
							auto tl = calcASTNode(l);
							if (tl->getObjtype() == T_String) {
								liststr.push_back(tl->getdata<String>());
							}
						}
						tmp = new ASTNode(AST_DATA);
						tmp->setdata(Objects::sum(liststr));
						result = tmp;
					}
					break;
				case 11: // Print
					if (types == T_String)
						Objects::print(calcASTNode(list.at(0))->getdata<String>());
					else if (types == T_Number)
						Objects::print(calcASTNode(list.at(0))->getdata<Number>());
					break;
				case 2: // Sub
					Number num = calcASTNode(list.at(0))->getdata<Number>();
					for (auto p = list.begin() + 1; p != list.end(); ++p)
						num.sub(calcASTNode(*p)->getdata<Number>());
					tmp = new ASTNode(AST_DATA);
					tmp->setdata(num);
					result = tmp;
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
