#include "keyword.h"
#include "function.h"

ICM::FuncTable DefFuncTable;
ICM::FuncTable AddFuncTable;
ICM::VariableTable DefVariableTable;
ICM::VariableTable AddVariableTable;

namespace ICM
{
	// Create Default FuncTable
	void createDefFuncTable()
	{
		using namespace ICM::Objects::Func;

		DefFuncTable.add("+", add, FuncParameter(FPT_VaryIdentical, 0, { T_Vary }));
		DefFuncTable.add("-", sub, FuncParameter(FPT_VaryIdentical, 0, { T_Number }));
		DefFuncTable.add("*", mul, FuncParameter(FPT_VaryIdentical, 0, { T_Number }));
		DefFuncTable.add("/", div, FuncParameter(FPT_VaryIdentical, 0, { T_Number }));
		DefFuncTable.add("%", mod, FuncParameter(FPT_VaryIdentical, 0, { T_Number }));
		DefFuncTable.add("=", equ, FuncParameter(FPT_Vary, 0));
		DefFuncTable.add("print", print, FuncParameter(FPT_Vary, 0));
		DefFuncTable.add("list", list, FuncParameter(FPT_Vary, 0));
		DefFuncTable.add("let", let, FuncParameter(FPT_Fixed, 2, { T_Identifier, T_Vary }));
	}
}
