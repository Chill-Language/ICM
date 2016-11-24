#include "basic.h"
#include "parser.h"
#include "config.h"
#include "interpreter.h"
#include "instruction.h"
#include "transform.h"
#include "timer.h"

extern size_t CheckCallCount;
using namespace ICM;

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
}

ICM::Config GlobalConfig(false, true, false);

void printIntervalTime(Timer &t)
{
	println("Current Interval Time : ", t.detTime());
}

vector<Instruction::InstructionList> Compile(const char *text)
{
	GlobalElementObjectPool.clear();
	vector<Instruction::InstructionList> VeI;

	Parser::Match match(text);
	while (!match.isend()) {
		AST ast;
		bool result = Parser::createAST(match, ast);
		if (result && !ast.empty()) {
			/*if (LoopMatch) {
			if (GlobalConfig.PrintAST)
			println("\nAST: \n", to_string(ast), "\n");
			if (GlobalConfig.PrintResult && GlobalConfig.PrintFlagWord)
			println("Output: ");
			}*/
			vector<AST::NodePtr> &Table = ast.getTable();
			Compiler::transform(Table);
			auto instlist = Compiler::createInstruction(Table);
			VeI.push_back(instlist);
		}
		else {
			//println("compile error.");
			break;
		}
	}

	return VeI;
}

void Run(vector<Instruction::InstructionList> &VeI, bool printResult)
{
	Timer t;

	for (auto &instlist : VeI) {
		Object *result = ICM::Run(instlist);

		if (printResult)
			println("=> ", result ? result->to_output() : "Null", "\n");

		if (GlobalConfig.PrintIntervalTime)
			printIntervalTime(t);
	}
}

int main(int argc, char *argv[])
{
	// Initialize
	createDefFuncTable();
	const bool LoopMatch = (argc <= 1);
	//const bool LoopMatch = false;

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
		println("ICM 0.3");
	}
	else {
		File file(argv[1], File::Text);
		if (file.bad()) {
			println("Error in open file ", argv[1], ".");
			return -1;
		}
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
		auto instlist = Compile(text);
		Run(instlist, LoopMatch);

	} while (LoopMatch);

	return 0;
}
