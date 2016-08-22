#include "basic.h"
#include "parser.h"
#include "runast.h"
#include "tostring.h"
#include "keyword.h"
#include "function.h"
#include "file.h"
#include "config.h"

using namespace ICM;

void test()
{
}

#include <ctime>

class Timer
{
public:
	Timer() : time(clock()) {}
	size_t detTime() {
		clock_t currTime = clock();
		size_t det = currTime - time;
		time = currTime;
		return det;
	}

	clock_t time;
};

ICM::Config GlobalConfig(false, true, false);

void printIntervalTime(Timer &t)
{
	println("Current Interval Time : ", t.detTime());
}

int main(int argc, char *argv[])
{
	Timer t;
	// Initialize
	createDefFuncTable();
	const bool LoopMatch = argc <= 1;

	if (GlobalConfig.PrintIntervalTime)
		printIntervalTime(t);

	string init_text;

	// Load File
	if (LoopMatch) {
		println("ICM 0.1");
	}
	else {
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
					if (GlobalConfig.PrintAST)
						println("\nAST: \n", ast, "\n");
					if (GlobalConfig.PrintResult && GlobalConfig.PrintFlagWord)
						println("Output: ");
				}
				ASTNode *data = runAST(ast);
				if (LoopMatch) {
					if (GlobalConfig.PrintResult) {
						print(GlobalConfig.PrintFlagWord ? "\n\nResult:\n" : "=> ");
						if (data && data->getNodeType() == AST_DATA)
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
			if (GlobalConfig.PrintIntervalTime)
				printIntervalTime(t);
		}
	} while (LoopMatch);

	if (GlobalConfig.PrintIntervalTime)
		printIntervalTime(t);

	return 0;
}
