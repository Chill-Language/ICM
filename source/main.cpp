#include "prints.h"
#include "parser.h"
#include "objects.h"
#include "tostring.h"
#include "objects.h"
#include <map>
using namespace Common;
using namespace ICM;

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
		auto types = list.front()->getObjtype();
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
					tmp->setdata<Number>(Objects::sum(listnum));
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
					tmp->setObjtype(T_String);
					tmp->setdata<String>(Objects::sum(liststr));
					result = tmp;
				}
				break;
			case 11: // Print
				Objects::print(calcASTNode(list.at(0))->getdata<String>());
				break;
			case 2: // Sub
				Number num = calcASTNode(list.at(0))->getdata<Number>();
				for (auto p = list.begin() + 1; p != list.end(); ++p)
					num.sub(calcASTNode(*p)->getdata<Number>());
				tmp = new ASTNode(AST_DATA);
				tmp->setdata<Number>(num);
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
	Common::Output::print("Output: \n");
	calcASTNode(ast->getRoot());
	println();
}


int main(void)
{
	const char *text = "(print (+ \"Hello \" \"World!\"))";

	Common::Output::print("Input: \n");
	Common::Output::println(text);
	println();



	KeyWordMap KeyWords {
		KeyWord("+", 1),
		KeyWord("-", 2),
		KeyWord("*", 3),
		KeyWord("/", 4),
		KeyWord("=", 5),
		KeyWord(">", 6),
		KeyWord("<", 7),
		KeyWord(">=", 8),
		KeyWord("<=", 9),
		KeyWord("list",10),
		KeyWord("print",11),
	};

	// Parser

	Match match(text);

	while (!match.isend()) {
		AST *ast = createAST(match, KeyWords);
		Common::Output::print("AST: \n");
		println(ast);
		println();
		runAST(ast);
		delete ast;
	}
	println();

	return 0;
}
