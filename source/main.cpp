#include "basic.h"
#include "parser.h"
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
						if (front->isData()) {
							ObjectPtr op(front->getData());
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
	string str("(sort [1 2 3] >)");
	//string str("(+ 5 6)");
	//string str("(while (not (not F)) F F F)");
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
	//const auto &n1 = ast.getRoot();
	//auto &n2 = ast.getTable();
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

void test2()
{
	Function::SignTree Tree;
	auto &ftu = DefFuncTable["+"];
	for (auto i : range(0, ftu.size())) {
		Tree.insert(ftu[i]);
	}
	Function::SignTreeMatch STM(Tree);
	auto r = STM.match({ T_Number, T_Number, T_Number });

	println(r);
	return;
}

namespace ICM
{
	namespace Types
	{
		struct TypeObject
		{
		public:
			enum Property {
				Const,
				Static,
				Mult,
				Repeat,
				Volatile,
			};
		public:
			bool isconst() const { return getProperty<Const>(); }
			bool isstatic() const { return getProperty<Static>(); }
			bool ismult() const { return getProperty<Mult>(); }
			bool isrepeat() const { return getProperty<Repeat>(); }
			bool isvolatile() const { return getProperty<Volatile>(); }

			void setconst() { setProperty<Const>(); }
			void setstatic() { setProperty<Static>(); }
			void setmult() { setProperty<Mult>(); }
			void setrepeat() { setProperty<Repeat>(); }
			void setvolatile() { setProperty<Volatile>(); }

		public:
			TypeUnit data = T_Null;
		private:
			size_t property = 0;
			union {
				void* ptr;
				size_t idx;
			} extand;

		private:
			template <Property _P>
			bool getProperty() const {
				return (property & (1 << _P)) >> _P != 0;
			}
			template <Property _P>
			void setProperty() {
				property |= (1 << _P);
			}

		};

		string to_string(const TypeObject &to) {
			string str;
			str.append(to_string((DefaultType)to.data));
			if (to.isconst() || to.isstatic() || to.isvolatile()) {
				str.push_back('.');
				if (to.isconst())
					str.push_back('c');
				if (to.isstatic())
					str.push_back('s');
				if (to.isvolatile())
					str.push_back('v');
			}
			if (to.ismult()) {
				str.append("()");
			}
			if (to.isrepeat())
				str.push_back('*');

			return str;
		}
	}

	namespace FunctionNew
	{
		struct Signature
		{
			Signature(const std::initializer_list<Types::TypeObject> &inil, const Types::TypeObject &out)
				: InTypes(inil), OutType(out) {}
			lightlist<Types::TypeObject> InTypes;
			Types::TypeObject OutType;
		};

		string to_string(const Signature &sign) {
			string str;
			auto &InTypes = sign.InTypes;
			auto &OutType = sign.OutType;
			if (InTypes.size() > 1) {
				str.push_back('(');
				for (auto &t : InTypes) {
					str.append(to_string(t));
					str.push_back(' ');
				}
				str.pop_back();
				str.push_back(')');
			}
			else if (InTypes.size() == 1) {
				str.append(to_string(InTypes.front()));
			}
			else {
				str.append("Void");
			}
			str.append(" -> ");
			str.append(to_string(OutType));
			return str;
		}
	}
}

void test()
{
	/*Types::TypeObject to;
	to.data = T_Number;
	//to.setmult();
	//to.setconst();
	//to.setstatic();
	//to.setrepeat();
	to.setvolatile();
	println(to);

	FunctionNew::Signature sign({ to, to }, to);
	println(sign);
	*/

	//test2();
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
			if (result && !ast.empty()) {
				if (LoopMatch) {
					if (GlobalConfig.PrintAST)
						println("\nAST: \n", to_string_code(ast), "\n");
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
