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
	// Initialize
	createDefaultFuncTable();
	const bool LoopMatch = !true;

	// Test
	test();

	// Init Text
	const char *init_text = "(print (+ \"Hello \" \"World!\"))";

	charptr text(LoopMatch ? charptr(0xff) : charptr(init_text));

	// Loop
	do {
		// Input
		if (LoopMatch) {
			print("> ");
			fgets((char*)text, 0xff, stdin);
			text[text.length() - 1] = '\0';
		}
		else {
			print("Input: \n");
			println(text.to_string());
		}
		println();

		// Main
		Match match(text);
		while (!match.isend()) {
			AST *ast = Parser::createAST(match);
			if (ast && ast->getRoot()) {
				print("AST: \n");
				println(ast);
				println();
				print("Output: \n");
				runAST(ast);
				println();
				delete ast;
			}
			else {
				break;
			}
		}
	} while (LoopMatch);

	return 0;
}
