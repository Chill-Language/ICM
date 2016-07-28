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
	createFuncMap();
	createKeyWords();

	const bool LoopMatch = true;

	// Init Text
	const char *init_text = "(print (+ \"Hello \" \"World!\"))";
	//const char *init_text = "(2 + 5)";
	//const char *init_text = "(/ 5 0)";
	//const char *init_text = "(+ + +)";

	charptr text(LoopMatch ? charptr(0xff) : charptr(init_text));

	// Test
	test();

	// Loop
	do {
		print("> ");
		if (LoopMatch) {
			fgets((char*)text, 0xff, stdin);
			text[text.length() - 1] = '\0';
		}

		Match match(text);

		// Main

		print("Input: \n");
		println(text.to_string());
		println();

		while (!match.isend()) {
			AST *ast = Parser::createAST(match, KeyWords);
			if (ast != nullptr) {
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
