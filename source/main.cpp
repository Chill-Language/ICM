#include "basic.h"
#include "parser.h"
#include "tostring.h"
#include "keyword.h"
#include "function.h"
#include "file.h"
#include "config.h"
#include "order.h"
#include "interpreter.h"

#include "objectsdef.h"
using namespace ICM;

void testSub()
{
	string str("((disp [+]))");

	//string str("(+ (+ 5 6) (- 7 2))");
	//string str("(if T then 2 else 3)");
	//string str("(if (& (= a 2) (< b 3)) then (let a 5) (+ a (- 7 (* 3 8)) (/ 2 5)) else if (let b 7) then (+ b 7))");

	Match match(str.c_str());
	shared_ptr<AST> ast = Parser::createAST(match);
	vector<AST::NodePtr> table = ast->getTable();
	ASTOrder::CreateOrder createorder(table);
	const auto &e = createorder.createOrderASTs();
	Interpreter interpreter(e);
	const auto &op = interpreter.run();
	println(op);

}

void test()
{
	//testSub();
	//exit(0);
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
	//test();
	//exit(0);

	Timer t;
	// Initialize
	createDefFuncTable();
	createDefKeywordTable();
	const bool LoopMatch = (argc <= 1);
	//const bool LoopMatch = false;

	if (GlobalConfig.PrintIntervalTime)
		printIntervalTime(t);

	//GlobalConfig.SetDebugMode(true);
	//GlobalConfig.PrintAST = true;
	//GlobalConfig.PrintOrder = true;

	// Test
	test();

	string init_text;

	// Load File
	if (LoopMatch) {
		println("ICM 0.2");
	}
	else {
		File file(argv[1], File::Text);
		init_text = file.getText();
	}

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
			shared_ptr<AST> ast = Parser::createAST(match);
			if (ast && ast->getRoot()) {
				if (LoopMatch) {
					if (GlobalConfig.PrintAST)
						println("\nAST: \n", ast->to_string_code(), "\n");
					if (GlobalConfig.PrintResult && GlobalConfig.PrintFlagWord)
						println("Output: ");
				}
				vector<AST::NodePtr> table = ast->getTable();
				ASTOrder::CreateOrder createorder(table);
				const auto &e = createorder.createOrderASTs();
				Interpreter interpreter(e);
				const auto &op = interpreter.run();
				if (LoopMatch) {
					if (GlobalConfig.PrintResult) {
						print(GlobalConfig.PrintFlagWord ? "\n\nResult:\n" : "=> ");
						println(op->to_string());
						println();
					}
				}
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
