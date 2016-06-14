#include "prints.h"
#include "parser.h"
#include "memory.h"
#include "tostring.h"
#include <map>
using namespace Common;
using namespace ICM;

const ASTNode* calcASTNode(const ASTNode *node)
{
	const ASTNode* result = nullptr;
	ASTNode *tmp = nullptr;
	int num = 0;
	std::string str;

	switch (node->getNodeType()) {
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
					num = 0;
					for (auto &l : list) {
						auto tl = calcASTNode(l);
						if (tl->getObjtype() == T_Number)
							num += tl->getdata<int>();
					}
					tmp = new ASTNode(AST_DATA);
					tmp->setdata<int>(num);
					result = tmp;
				}
				else if (types == T_String) {
					for (auto &l : list) {
						auto tl = calcASTNode(l);
						if (tl->getObjtype() == T_String) {
							str += tl->getdata<std::string>();
						}
					}
					tmp = new ASTNode(AST_DATA);
					tmp->setObjtype(T_String);
					tmp->setdata<std::string>(str);
					result = tmp;
				}
				break;
			case 2: // Sub
				num = calcASTNode(list.at(0))->getdata<int>();
				for (auto p = list.begin() + 1; p != list.end(); ++p)
					num -= calcASTNode(*p)->getdata<int>();
				tmp = new ASTNode(AST_DATA);
				tmp->setdata<int>(num);
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
	println(ast);
	println(calcASTNode(ast->getRoot()));
}


int main(void)
{
	const char *text = "(+ \"Hello \" \"World!\")";

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
	};

	// Parser

	Match match(text);

	while (!match.isend()) {
		AST *ast = createAST(match, KeyWords);
		runAST(ast);
		delete ast;
	}

	return 0;
}
