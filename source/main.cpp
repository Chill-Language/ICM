#include "basic.h"
#include "parser.h"
#include "runast.h"
#include "tostring.h"
#include "keyword.h"
#include "function.h"

using namespace ICM;

void test()
{
}

int main(void)
{
	const bool LoopMatch = true;
	//const char *text = "(print (+ \"Hello \" \"World!\"))";

	createFuncMap();
	createKeyWords();

	// Test
	test();

	charptr text(0xff);

	do {
		if (LoopMatch)
			fgets((char*)text, 0xff, stdin);
		text[text.length() - 1] = '\0';

		Match match(text);

		// Main

		print("Input: \n");
		println(text.to_string());
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
	} while (LoopMatch);

	return 0;
}
