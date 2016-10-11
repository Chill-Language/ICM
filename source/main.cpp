#include "basic.h"
#include "parser.h"
#include "tostring.h"
#include "file.h"
#include "config.h"
#include "order.h"
#include "interpreter.h"

#include "objectsdef.h"
extern size_t CheckCallCount;
using namespace ICM;

namespace ICM
{
	namespace ASTOrder
	{
		class Optimization
		{
		public:
			Optimization(const vector<OrderData*> &List)
				: List(List) {}

			void start() {
				size_t Count = 0;
				vector<DefaultType> Types(List.size());
				while (Count < List.size()) {
					auto *o = List[Count];
					println("Doing: ", o->to_string());
					switch (o->order())
					{
					case OrderData::CCAL: {
						Types[Count] = T_Vary;
						OrderDataCheckCall* p = static_cast<OrderDataCheckCall*>(o);
						AST::Node *node = p->getData();
						AST::Element *front = (AST::Element*)&(node->front());
						if (front->getType() == AST::Element::E_Data) {
							ObjectPtr op = static_cast<AST::Element*>(front)->getData();
							if (op.isType(T_Function)) {
								Objects::Function *of = op.get<Objects::Function>();
								//getCallID(of->get_data(), );
							}
						}
						else {

						}
							break;
					}
					default:
						NewList.push_back(o);
						println(o->to_string());
						break;
					}
					Count++;
				}
			}
			const vector<OrderData*>& getNewList() const {
				return NewList;
			}

		private:
			const vector<OrderData*>& List;
			vector<OrderData*> NewList;
			map<size_t, size_t> ReferMap;
		};
	}
}

void testSub()
{
	//string str("(+ 5 6)");
	string str("(while (not (not F)) F F F)");
	//string str("(if (if (+ 5) then (+ 3) else (+ 2)) then (+ 0) else (+ 1))");
	//string str("(loop 4 (loop 5) 6)");
	//string str("(p (if (= 3 4) then (+ 6 4) else (if (= 3 4) then (+ 2 6) else (+ 0 1))))");
	//string str("(p (if (= 3 4) then (+ 2 6) else (+ 6 4)))");
	//string str("(if (= 3 4) then (- 2 5) (+ 2 6) elsif (+ 0 1) then (+ 5 6) else (- 5 2) (+ 6 4))");
	//string str("(if (- (* 3 2) 4) then (- 2 5) (+ (+ (* 3 2) 6) (- 7 2)) else (- 5 2) (+ 6 4))");
	//string str("(if T then 7 else 3)");
	//string str("(if (= 5 6) then 7 else 3)");
	//string str("((disp [+]) (+ 5 6))");
	//string str("(+ (+ (- 6 4) (* 2 3)) (- 7 2))");
	//string str("(if T then 2 else 3)");
	//string str("(if (& (= a 2) (< b 3)) then (let a 5) (+ a (- 7 (* 3 8)) (/ 2 5)) else if (let b 7) then (+ b 7))");

	println(str);

	Match match(str.c_str());
	AST ast;
	bool result = Parser::createAST(match, ast);
	//println(result);
	//return;
	const auto &n1 = ast.getRoot();
	auto &n2 = ast.getTable();
	//oast.to_string();
	vector<AST::NodePtr> table = ast.getTable();
	ASTOrder::CreateOrder createorder(table);
	const auto *p = &createorder.createOrder();
	//ASTOrder::Optimization opt(*p);
	//opt.start();
	//p = &opt.getNewList();


	//if (false)
	{
		Interpreter interpreter(*p);
		const auto &op = interpreter.run();
		println(op);
	}

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
		size_t det = (size_t)(currTime - time);
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
	const bool LoopMatch = (argc <= 1);
	//const bool LoopMatch = false;

	if (GlobalConfig.PrintIntervalTime)
		printIntervalTime(t);

	//GlobalConfig.SetDebugMode(true);
	if (false)
	{
		GlobalConfig.PrintAST = true;
		GlobalConfig.PrintOrder = true;
	}
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
			AST ast;
			bool result = Parser::createAST(match, ast); // TODO
			if (result && ast.getRoot()) {
				if (LoopMatch) {
					if (GlobalConfig.PrintAST)
						println("\nAST: \n", ast.to_string_code(), "\n");
					if (GlobalConfig.PrintResult && GlobalConfig.PrintFlagWord)
						println("Output: ");
				}
				vector<AST::NodePtr> table = ast.getTable();
				ASTOrder::CreateOrder createorder(table);
				const auto &e = createorder.createOrder();
				Interpreter interpreter(e);
				const ObjectPtr &op = interpreter.run();
				if (LoopMatch) {
					if (GlobalConfig.PrintResult) {
						print(GlobalConfig.PrintFlagWord ? "\n\nResult:\n" : "=> ");
						println(op.to_string());
						println();
					}
				}
				//println(CheckCallCount);
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
