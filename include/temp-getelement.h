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
			dat = new Compiler::Literal::StringType(static_cast<char*>(dat)); // TODO
		}
		else if (elt.getDataType() == T_Nil) {
			dat = nullptr;
		}
		TypeUnit type = elt.getDataType();
		return Object(type, TypeInfoTable[type].copy(dat)); // TODO
	}
	inline const string& getIdent(const AST::Element &elt) {
		assert(elt.isIdent());
		return Compiler::GlobalIdentNameMap.getKey(elt.getIndex());
	}
#if _USE_IDENTTABLE_
	inline void setIdent(AST::Element &elt, IdentType type, size_t index) {
		elt = AST::Element::Identifier(type, index);
	}
	inline DyVarbTableUnit& getDyVarb(const AST::Element &elt) {
		assert(elt.isIdentType(I_DyVarb));
		return GlobalIdentTable.at(elt.getIndex()).DyVarb;
	}
	inline FuncTableUnit& getFunction(const AST::Element &elt) {
		assert(elt.isIdentType(I_Function));
		return GlobalFunctionTable[GlobalIdentTable.at(elt.getIndex()).FunctionIndex];
	}
#else
	inline void setDyVarb(AST::Element &elt, size_t index) {
		elt = AST::Element::Identifier(I_DyVarb, index);
	}
	inline DyVarbTableUnit& getDyVarb(const AST::Element &elt) {
		assert(elt.isIdentType(I_DyVarb));
		return GlobalDyVarbTable[elt.getIndex()];
	}
	inline void setFunction(AST::Element &elt, size_t index) {
		elt = AST::Element::Identifier(I_Function, index);
	}
	inline FuncTableUnit& getFunction(const AST::Element &elt) {
		assert(elt.isIdentType(I_Function));
		return GlobalFunctionTable[elt.getIndex()];
	}
#endif
}
