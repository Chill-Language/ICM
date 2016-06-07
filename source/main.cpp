#include "prints.h"
#include "ast.h"
#include "match.h"
using namespace System;

template <typename T>
void println(const T *obj)
{
	Output::println(to_string(obj));
}
template <typename T>
void println(const T &obj)
{
	Output::println(to_string(obj));
}

using namespace ICM;

int main(void)
{
	AST *ast;

	const char *text = "(+ 5 6 ); sss";
	Match match(text);

	ast = new AST();
	char *begin = nullptr;

	MatchResult mr;
	println(match.matchNext());
	println(match.matchNext());
	println(match.matchNext());
	println(match.matchNext());
	println(match.matchNext());
	/*while (mr.begin() != mr.end()) {
		println(mr);
		if (mr.getType() == T_LBracket)
			ast->pushNode(AST_FUNC);
		if (mr.getType() == T_RBracket)
			ast->retNode();
		if (mr.getType() == T_Identifier)
			ast->setfunc(FUNC_DEF, mr.getAddMsg());
		match.matchNext();
	}*/
	println(ast);

	/*
	// (+ 5 6)
	ast = new AST();
	
	ast->pushNode(AST_FUNC)->setfunc(FUNC_DEF, 1); // ( +
	ast->pushNode(AST_DATA)->setdata(5);           // 5
	ast->pushNode(AST_DATA)->setdata(6);           // 6
	ast->retNode();                                // )

	println(ast);
	delete ast;

	// (+ 7 (+ 5 6))
	ast = new AST();

	ast->pushNode(AST_FUNC)->setfunc(FUNC_DEF, 1); // ( +
	ast->pushNode(AST_DATA)->setdata(7);           // 7
	ast->pushNode(AST_FUNC)->setfunc(FUNC_DEF, 1); // ( +
	ast->pushNode(AST_DATA)->setdata(5);           // 5
	ast->pushNode(AST_DATA)->setdata(6);           // 6
	ast->retNode();                                // )
	ast->retNode();                                // )

	println(ast);
	delete ast;
	*/

	return 0;
}
