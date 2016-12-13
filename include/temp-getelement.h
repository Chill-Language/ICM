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
			dat = new string(static_cast<char*>(dat));
		}
		else if (elt.getDataType() == T_Nil) {
			dat = nullptr;
		}
		// TODO
		return *Object(elt.getDataType(), dat).clone();
	}
	inline const string& getIdentifier(const AST::Element &elt) {
		assert(elt.isIdentifier());
		return GlobalIdentifierMap.getKey(elt.getIndex());
	}
	inline VariableTableUnit& getVariable(const AST::Element &elt) {
		assert(elt.isVariable());
		return GlobalVariableTable[elt.getIndex()];
	}
	inline FuncTableUnit& getFunction(const AST::Element &elt) {
		assert(elt.isFunction());
		return GlobalFunctionTable[elt.getIndex()];
	}
}
