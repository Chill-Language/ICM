#include "prints.h"
#include "ast.h"
#include <stack>
using std::string;
using std::vector;
using std::stack;
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

	// (+ 5 6)
	ast = new AST();
	
	ast->pushNode(AST_FUNC)->setfunc(FUNC_DEF, 1);
	ast->pushNode(AST_DATA)->setdata(5);
	ast->pushNode(AST_DATA)->setdata(6);
	ast->retNode();

	println(ast);
	delete ast;

	// (+ 7 (+ 5 6))
	ast = new AST();

	ast->pushNode(AST_FUNC)->setfunc(FUNC_DEF, 1);
	ast->pushNode(AST_DATA)->setdata(7);
	ast->pushNode(AST_FUNC)->setfunc(FUNC_DEF, 1);
	ast->pushNode(AST_DATA)->setdata(5);
	ast->pushNode(AST_DATA)->setdata(6);
	ast->retNode();
	ast->retNode();

	println(ast);
	delete ast;

	return 0;
}
