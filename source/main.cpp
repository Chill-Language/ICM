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
	const bool DebugMode = !true;
	const bool PrintAST = DebugMode;
	const bool PrintFlagWord = DebugMode;
	const bool PrintResult = true;
	string init_text;

	// Load File
	if (!LoopMatch) {
		File file(argv[1], "rt");
		init_text = file.get_text();
	}

	// Test
	test();

	// Init Text
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
					if (PrintAST)
						println("\nAST: \n", ast, "\n");
					if (PrintResult && PrintFlagWord)
						println("Output: ");
				}
				ASTNode *data = runAST(ast);
				if (LoopMatch) {
					if (PrintResult) {
						print(PrintFlagWord ? "\n\nResult:\n" : "=> ");
						if (data->getNodeType() == AST_DATA)
							println(to_string(data->getdata()));
						else
							println(to_string(data));
						println();
					}
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
