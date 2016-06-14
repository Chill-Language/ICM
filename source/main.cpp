#include "prints.h"
#include <map>
#include "parser.h"
using namespace Common;
using namespace ICM;

template <typename T>
void println(const T &obj)
{
	Output::println(to_string(obj));
}


const ASTNode* calcASTNode(const ASTNode *node)
{
	const ASTNode* result = nullptr;
	ASTNode *tmp = nullptr;
	int num = 0;

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
		if (type == FUNC_DEF) {
			// TODO
			switch (id) {
			case 1: // Add
				num = 0;
				for (auto &l : list)
					num += calcASTNode(l)->getdata<int>();
				tmp = new ASTNode(AST_DATA);
				tmp->setdata<int>(num);
				result = tmp;
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
	const char *text = "(+ 5 6 (- 7 6))";

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
