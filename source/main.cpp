#include "basic.h"
#include "parser.h"
#include "runast.h"
#include "tostring.h"
#include "keyword.h"
#include "function.h"

using namespace ICM;

//namespace ICM
//{
	extern ICM::KeyWordMap KeyWords;
//}

void test()
{
}

int main(void)
{
	const char *text = "(print (+ \"Hello \" \"World!\"))";
	//const char *text = "(* 100 5 5)";
	//const char *text = "(+ 1 \"7\")";

	Match match(text);

	createFuncMap();
	createKeyWords();

	// Test
	test();

	// Main

	print("Input: \n");
	println(text);
	println();

	while (!match.isend()) {
		AST *ast = Parser::createAST(match, KeyWords);
		print("AST: \n");
		println(ast);
		println();
		print("Output: \n");
		runAST(ast);
		println();
		delete ast;
	}

	println();

	return 0;
}
