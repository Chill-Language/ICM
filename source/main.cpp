#include "basic.h"
#include "parser.h"
#include "runast.h"
#include "tostring.h"
#include "keyword.h"
#include "function.h"
#include "file.h"

using namespace ICM;

void test()
{
}

int main(int argc, char *argv[])
{
	// Initialize
	createDefFuncTable();
	const bool LoopMatch = argc <= 1;
	string init_text;

	if (!LoopMatch) {
		File file(argv[1], "rt");
		init_text = file.get_text();
	}

	// Test
	test();

	// Init Text
	//const char *init_text = "(print (list 5 6 7) \" \" 3)";
	//const char *init_text = "(print (+ \"Hello \" \"World!\"))";

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
			//println("Input: \n", text.to_string());
		}

		// Main
		Match match(text);
		while (!match.isend()) {
			AST *ast = Parser::createAST(match);
			if (ast && ast->getRoot()) {
				if (LoopMatch) {
					println("\nAST: \n", ast, "\n");
					println("Output: ");
				}
				ASTNode *data = runAST(ast);
				if (LoopMatch) {
					println();
					println("\nResult:\n", data, "\n");
				}
				delete ast;
			}
			else {
				break;
			}
		}
	} while (LoopMatch);

	return 0;
}
