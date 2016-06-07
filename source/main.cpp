#include "prints.h"
#include "ast.h"
#include <map>
#include "match.h"
using namespace System;

template <typename T>
void println(const T &obj)
{
	Output::println(to_string(obj));
}


using namespace ICM;

int main(void)
{
	AST *ast;

	const char *text = "(+ 5 6) ; Add Two Numbers.";
	Match match(text);

	ast = new AST();

	MatchResult mr;
	mr = match.matchNext();
	bool firstMatchBraket = false;

	std::map<std::string, int> KeyWords {
		std::pair<std::string, int>("+", 1),
		std::pair<std::string, int>("-", 2),
		std::pair<std::string, int>("*", 3),
		std::pair<std::string, int>("/", 4),
		std::pair<std::string, int>("=", 5),
		std::pair<std::string, int>(">", 6),
		std::pair<std::string, int>("<", 7),
		std::pair<std::string, int>(">=", 8),
		std::pair<std::string, int>("<=", 9),
	};

	while (mr.begin() != mr.end()) {
		println(&mr);
		if (mr.getType() == T_LBracket) {
			ast->pushNode(AST_FUNC);
			firstMatchBraket = true;
		}
		if (mr.getType() == T_RBracket) {
			ast->retNode();
		}
		if (mr.getType() == T_Identifier) {
			if (firstMatchBraket) {
				ast->setfunc(FUNC_DEF, KeyWords[mr.getString().c_str()]);
			}
			else
				ast->pushNode(AST_DATA)->setdata(mr.getString());
			firstMatchBraket = false;
		}
		if (mr.getType() == T_Number) {
			ast->pushNode(AST_DATA)->setdata(atoi(mr.getString().c_str()));
		}
		mr = match.matchNext();
	}
	println(ast);

	return 0;
}
