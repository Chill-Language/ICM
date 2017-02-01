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
		assert(elt.isIdentType(I_Void));
		return Compiler::GlobalIdentNameMap.getKey(elt.getIndex());
	}
	inline void setIdent(AST::Element &elt, IdentType type, size_t index) {
		elt = AST::Element::Identifier(type, index);
	}

	inline Object* getDyVarbData(size_t ident_index) {
		assert(GlobalIdentTable.at(ident_index).type == I_DyVarb);
		return &GlobalIdentTable.at(ident_index).DyVarb;
	}
	inline void setDyVarbData(size_t ident_index, Object &&data) {
		*getDyVarbData(ident_index) = data;
	}
	inline void setDyVarbData(size_t ident_index, Object *data) {
		*getDyVarbData(ident_index) = *data;
	}

	inline const string& getIdentName(size_t ident_index) {
		return GlobalIdentTable.getName(ident_index);
	}

	inline size_t getIdentID(const AST::Element &elt) {
		assert(!elt.isIdentType(I_Void));
		return elt.getIndex();
	}
	inline const string& getIdentName(const AST::Element &elt) {
		assert(!elt.isIdentType(I_Void));
		return getIdentName(elt.getIndex());
	}
	inline Object* getDyVarbData(const AST::Element &elt) {
		assert(elt.isIdentType(I_DyVarb));
		return getDyVarbData(elt.getIndex());
	}
	inline FuncTableUnit& getFunction(const AST::Element &elt) {
		assert(elt.isIdentType(I_Function));
		return GlobalFunctionTable[GlobalIdentTable.at(elt.getIndex()).FunctionIndex];
	}
}
