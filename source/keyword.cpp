#include "keyword.h"
#include "function.h"
#include "deffunc.h"
#include "objectsdef.h"
#include "number.h"

ICM::FuncTable DefFuncTable;
ICM::FuncTable AddFuncTable;
ICM::VariableTable DefVariableTable;
ICM::VariableTable AddVariableTable;

namespace ICM
{
	// Create Default FuncTable
	void createDefFuncTable()
	{
		using namespace ICM::DefFunc;

		DefFuncTable.add("+", add, FuncParameter(FPT_VaryIdentical, 0, { T_Vary }));
		DefFuncTable.add("-", sub, FuncParameter(FPT_VaryIdentical, 0, { T_Number }));
		DefFuncTable.add("*", mul, FuncParameter(FPT_VaryIdentical, 0, { T_Number }));
		DefFuncTable.add("/", div, FuncParameter(FPT_VaryIdentical, 0, { T_Number }));
		DefFuncTable.add("mod", mod, FuncParameter(FPT_VaryIdentical, 0, { T_Number }));
		DefFuncTable.add("rem", rem, FuncParameter(FPT_VaryIdentical, 0, { T_Number }));
		DefFuncTable.add("=", equ, FuncParameter(FPT_Vary, 0));
		DefFuncTable.add("<", n_small, FuncParameter(FPT_Fixed, 2, { T_Number, T_Number }));
		DefFuncTable.add("<=", n_smallequal, FuncParameter(FPT_Fixed, 2, { T_Number, T_Number }));
		DefFuncTable.add(">", n_large, FuncParameter(FPT_Fixed, 2, { T_Number, T_Number }));
		DefFuncTable.add(">=", n_largeequal, FuncParameter(FPT_Fixed, 2, { T_Number, T_Number }));
		DefFuncTable.add("++", inc, FuncParameter(FPT_Fixed, 1, { T_Number }));
		DefFuncTable.add("inc", inc, FuncParameter(FPT_Fixed, 1, { T_Number }));
		DefFuncTable.add("--", dec, FuncParameter(FPT_Fixed, 1, { T_Number }));
		DefFuncTable.add("dec", dec, FuncParameter(FPT_Fixed, 1, { T_Number }));
		DefFuncTable.add("print", DefFunc::print, FuncParameter(FPT_Vary, 0));
		DefFuncTable.add("println", DefFunc::println, FuncParameter(FPT_Vary, 0));
		DefFuncTable.add("list", list, FuncParameter(FPT_Vary, 0));
		DefFuncTable.add("order", order, FuncParameter(FPT_Fixed, 1, { T_List }));
		DefFuncTable.add("let", let, FuncParameter(FPT_Fixed, 2, { T_Identifier, T_Vary }));
		DefFuncTable.add("dcall", dcall, FuncParameter(FPT_Fixed, 3, { T_Vary, T_Identifier, T_Vary }));
		DefFuncTable.add("exit", exit, FuncParameter());
		DefFuncTable.add("system", system, FuncParameter(FPT_Fixed, 1, { T_String }));

		// TODO
		Objects::Identifier *idp = new Objects::Identifier("Nil", new ASTNode(ObjectPtr(new Objects::Nil())));
		DefVariableTable.add("Nil", idp);
		DefVariableTable.add("NIL", idp);
		Objects::Identifier *nan = new Objects::Identifier("NaN", new ASTNode(ObjectPtr(new Objects::Number(Common::Number::Rational(0, 0)))));
		DefVariableTable.add("NaN", nan);
	}
}
