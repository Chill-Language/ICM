#include "prints.h"
#include "parser.h"
#include "runast.h"
#include <map>
using std::to_string;
using namespace Common;
using namespace ICM;

int main(void)
{
	const char *text = "(+ 5 6)";

	print("Input: \n");
	println(text);
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
