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

namespace ICM
{
	using KeyWordMap = std::map<std::string, int>;
	using KeyWord = KeyWordMap::value_type;

	AST* createAST(Match &match, const KeyWordMap &KeyWords)
	{
		AST *ast = new AST();
		MatchResult mr;

		bool firstMatchBraket = false;
		bool emptybreak = false;

		mr = match.matchNext();
		while (mr.begin() != mr.end()) {
			if (mr.getString()[0] != '\n') {
				println(&mr);
			}
			if (mr.getType() == T_LBracket) {
				ast->pushNode(AST_FUNC);
				firstMatchBraket = true;
				emptybreak = true;
			}
			else if (mr.getType() == T_RBracket) {
				ast->retNode();
			}
			else if (mr.getType() == T_Identifier) {
				if (firstMatchBraket) {
					ast->setfunc(FUNC_DEF, KeyWords.at(mr.getString()));
				}
				else {
					ast->pushNode(AST_DATA)->setdata(mr.getString());
				}
				firstMatchBraket = false;
			}
			else if (mr.getType() == T_Number) {
				ast->pushNode(AST_DATA)->setdata(atoi(mr.getString().c_str()))->settype(T_Number);
			}
			else if (mr.getType() == T_String) {
				ast->pushNode(AST_DATA)->setdata<std::string>(mr.getString())->settype(T_String);
			}
			if (emptybreak && ast->isend()) {
				break;
			}
			mr = match.matchNext();
		}
		return ast;
	}
}
using namespace ICM;

int main(void)
{
	const char *text = "(+ 5 6 (- 7 8)) ; Add Two Numbers. \n(+ 1 2)\n\n\n (+ 5 6)\n\n";

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
	};

	AST *ast;
	Match match(text);

	while (!match.isend()) {
		ast = createAST(match, KeyWords);
		println(ast);
		delete ast;
	}

	return 0;
}
