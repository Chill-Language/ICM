#include "basic.h"
#include "parser.h"
#include "runast.h"

using namespace ICM;

int main(void)
{
	//const char *text = "(+ \"Hello \" \"World!\")";
	const char *text = "(+ 5 7 9)";

	Match match(text);

	KeyWordMap KeyWords
	{
		KeyWord("+", 1),
		KeyWord("-", 2),
		KeyWord("*", 3),
		KeyWord("/", 4),
		KeyWord("%%", 5),
		KeyWord("=", 10),
		KeyWord(">", 11),
		KeyWord("<", 12),
		KeyWord(">=", 13),
		KeyWord("<=", 14),
		KeyWord("list",20),
		KeyWord("print",21),
	};

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
