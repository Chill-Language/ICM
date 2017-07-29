#include "basic.h"
#include "parser/literal.h"
#include "parser/parser.h"
#include "config.h"
#include "interpreter.h"
#include "instruction.h"
#include "transform.h"
#include "timer.h"

extern size_t CheckCallCount;
using namespace ICM;

namespace ICM
{
	void addDefFuncs(FuncTable &DefFuncTable);
	// Create Default FuncTable
	void createIdentTable()
	{
		addDefFuncs(GlobalFunctionTable);
		//
		for (const auto &var : GlobalFunctionTable) {
			IdentBasicIndex id = insertFromCurrentIdentTable(Compiler::GlobalIdentNameMap[var.first], I_StFunc);
			getFromCurrentIdentTable(id).FunctionIndex = GlobalFunctionTable.find(var.first);
		}
		setDyVarbData(insertGlobalDyVarbIdentIndex("true"), &Static.True);
		setDyVarbData(insertGlobalDyVarbIdentIndex("false"), &Static.False);
		// Import TypeInfoTable
		for (const auto &elt : TypeInfoTable) {
			const TypeInfo &info = elt.second;
			IdentBasicIndex id = insertFromCurrentIdentTable(Compiler::GlobalIdentNameMap[info.name], I_Type);
			getFromCurrentIdentTable(id).TypeIndex = info.index;
		}

		// TODO : Memory leak
		//GlobalDyVarbTable.insert("Nil");
		//DefVariableTable.insert("NIL", nil);
		//DefVariableTable.insert("nil", nil);
		//ObjectPtr nan(new Objects::Identifier("NaN", ObjectPtr(new Objects::Number(Common::Number::Rational(0, 0)))));
		//DefVariableTable.insert("NaN", nan);
	}

}

namespace ICM
{
	namespace Compiler
	{
		SerialBijectionMap<string> GlobalIdentNameMap;
	}
}

constexpr int_t func(int_t index, int_t size)
{
	//assert(index < size);
	//assert(index + size >= 0);
	return (index + size) % size;
}

void test()
{
	//exit(0);
}

ICM::Config GlobalConfig(false, true, false);

void printIntervalTime(Timer &t)
{
	println("Current Interval Time : ", t.detTime());
}

namespace ICM
{
	namespace Compiler
	{
		ICM::Instruction::InstructionList createInstruction(vector<AST::NodePtr> &Table);
	}
}

vector<Instruction::InstructionList> Compile(const char *text)
{
	Compiler::GlobalElementPool.clear();
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
	createIdentTable();
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
