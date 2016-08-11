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
		using F = Function::FuncObject;
		using S = Function::Signature;
		using Lst = std::initializer_list<F>;

		DefFuncTable.add("+", Lst{
			F(add, S({}, T_Number)),                     // Void -> N
			F(add, S({ T_Number }, T_Number)),           // N -> N
			F(add, S({ T_Number, T_Number }, T_Number)), // (N N) -> N
			F(add, S({ T_Number }, T_Number, true)),     // N* -> N
			F(add, S({ T_String }, T_String)),           // S -> S
			F(add, S({ T_String, T_String }, T_String)), // (S S) -> S
			F(add, S({ T_String }, T_String, true)),     // S* -> S
			F(add, S({ T_List }, T_List)),               // L -> L
			F(add, S({ T_List, T_List }, T_List)),       // (L L) -> L
			F(add, S({ T_List }, T_List, true)),         // L* -> L
		});
		DefFuncTable.add("-", Lst{
			F(sub, S({}, T_Number)),                     // Void -> N
			F(sub, S({ T_Number }, T_Number)),           // N -> N
			F(sub, S({ T_Number, T_Number }, T_Number)), // (N N) -> N
			F(sub, S({ T_Number }, T_Number, true)),     // N* -> N
		});
		DefFuncTable.add("*", Lst{
			F(mul, S({}, T_Number)),                     // Void -> N
			F(mul, S({ T_Number }, T_Number)),           // N -> N
			F(mul, S({ T_Number, T_Number }, T_Number)), // (N N) -> N
			F(mul, S({ T_Number }, T_Number, true)),     // N* -> N
		});
		DefFuncTable.add("/", Lst{
			F(div, S({}, T_Number)),                     // Void -> N
			F(div, S({ T_Number }, T_Number)),           // N -> N
			F(div, S({ T_Number, T_Number }, T_Number)), // (N N) -> N
			F(div, S({ T_Number }, T_Number, true)),     // N* -> N
		});
		DefFuncTable.add("mod", Lst{
			F(mod, S({ T_Number, T_Number }, T_Number)), // (N N) -> N
		});
		DefFuncTable.add("rem", Lst{
			F(rem, S({ T_Number, T_Number }, T_Number)), // (N N) -> N
		});
		DefFuncTable.add("equ", Lst{
			F(equ, S({ T_Vary, T_Vary }, T_Boolean)),       // (Var Var) -> Bool
			F(equ, S({ T_Vary, T_Vary }, T_Boolean, true)), // (Var Var*) -> Bool
		});
		DefFuncTable.add("<", Lst{
			F(n_small, S({ T_Number, T_Number }, T_Boolean)),       // (N N) -> Bool
			F(n_small, S({ T_Number, T_Number }, T_Boolean, true)), // (N N*) -> Bool
		});
		DefFuncTable.add("<=", Lst{
			F(n_smallequal, S({ T_Number, T_Number }, T_Boolean)),       // (N N) -> Bool
			F(n_smallequal, S({ T_Number, T_Number }, T_Boolean, true)), // (N N*) -> Bool
		});
		DefFuncTable.add(">", Lst{
			F(n_large, S({ T_Number, T_Number }, T_Boolean)),       // (N N) -> Bool
			F(n_large, S({ T_Number, T_Number }, T_Boolean, true)), // (N N*) -> Bool
		});
		DefFuncTable.add(">=", Lst{
			F(n_largeequal, S({ T_Number, T_Number }, T_Boolean)),       // (N N) -> Bool
			F(n_largeequal, S({ T_Number, T_Number }, T_Boolean, true)), // (N N*) -> Bool
		});
		DefFuncTable.add("inc", Lst{
			F(inc, S({ T_Identifier }, T_Identifier)), // I -> I
		});
		DefFuncTable.add("dec", Lst{
			F(dec, S({ T_Identifier }, T_Identifier)), // I -> I
		});
		DefFuncTable.add("print", Lst{
			F(DefFunc::print, S({}, T_List)),               // Void -> L
			F(DefFunc::print, S({ T_Vary }, T_List, true)), // Var* -> L
		});
		DefFuncTable.add("println", Lst{
			F(DefFunc::println, S({}, T_List)),               // Void -> L
			F(DefFunc::println, S({ T_Vary }, T_List, true)), // Var* -> L
		});
		DefFuncTable.add("list", Lst{
			F(list, S({}, T_List)),               // Void -> L
			F(list, S({ T_Vary }, T_List, true)), // Var* -> L
		});
		DefFuncTable.add("sort", Lst{
			F(order, S({ T_List }, T_List)), // L -> L
		});
		DefFuncTable.add("let", Lst{
			F(let, S({ T_Identifier }, T_Identifier)), // I -> I
		});
		DefFuncTable.add("cpy", Lst{
			//F(cpy, S({ T_Identifier }, T_Identifier)), // I -> I
		});
		DefFuncTable.add("dcall", Lst{
			F(dcall, S({ T_Vary, T_Function, T_Vary }, T_Vary)), // (Var F Var) -> Var
		});
		DefFuncTable.add("exit", Lst{
			F(exit, S({}, T_Nil)), // Void -> Nil
		});
		DefFuncTable.add("system", Lst{
			F(system, S({ T_String }, T_Number)), // S -> N
		});

		// TODO
		Objects::Identifier *idp = new Objects::Identifier("Nil", new ASTNode(ObjectPtr(new Objects::Nil())));
		DefVariableTable.add("Nil", idp);
		DefVariableTable.add("NIL", idp);
		Objects::Identifier *nan = new Objects::Identifier("NaN", new ASTNode(ObjectPtr(new Objects::Number(Common::Number::Rational(0, 0)))));
		DefVariableTable.add("NaN", nan);
	}
}
