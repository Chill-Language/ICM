#pragma once
#include "ast.h"
#include "literal.h"
#include "object.h"
#include "tabledata.h"

namespace ICM
{
	// Functions

	inline Object getData(const AST::Element &elt) {
		assert(elt.isData());
		void *dat = Compiler::GlobalElementPool.get(elt.getIndex());
		if (elt.getDataType() == T_String) {
			dat = new Compiler::Literal::StringType(static_cast<char*>(dat));
		}
		else if (elt.getDataType() == T_Nil) {
			dat = nullptr;
		}
		TypeUnit type = elt.getDataType();
		return Object(type, TypeInfoTable[type].copy(dat)); // TODO
	}
	inline const string& getIdent(const AST::Element &elt) {
		assert(elt.isIdent());
		return GlobalIdentMap.getKey(elt.getIndex());
	}
	inline VarbTableUnit& getVariable(const AST::Element &elt) {
		assert(elt.isIdentType(I_Variable));
		return GlobalVariableTable[elt.getIndex()];
	}
	inline FuncTableUnit& getFunction(const AST::Element &elt) {
		assert(elt.isIdentType(I_Function));
		return GlobalFunctionTable[elt.getIndex()];
	}
}
