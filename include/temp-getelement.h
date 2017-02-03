#pragma once
#include "ast.h"
#include "literal.h"
#include "object.h"
#include "tabledata.h"
#include "objectdef.h"

namespace ICM
{
	// Functions

	inline Object getLiteral(const AST::Element &elt) {
		assert(elt.isLiteral());
		void *dat = Compiler::GlobalElementPool.get(elt.getIndex());
		if (elt.getLiteralType() == T_String) {
			dat = new Compiler::Literal::StringType(static_cast<char*>(dat)); // TODO
		}
		else if (elt.getLiteralType() == T_Nil) {
			dat = nullptr;
		}
		TypeUnit type = elt.getLiteralType();
		return Object(type, TypeInfoTable[type].copy(dat)); // TODO
	}
	inline const string& getIdent(const AST::Element &elt) {
		assert(elt.isIdentType(I_Void));
		return Compiler::GlobalIdentNameMap.getKey(elt.getIndex());
	}
	inline void setIdent(AST::Element &elt, IdentType type, size_t index) {
		elt = AST::Element::Identifier(type, index);
	}

	// Ident

	inline size_t getIdentID(const AST::Element &elt) {
		assert(!elt.isIdentType(I_Void));
		return elt.getIndex();
	}
	inline const string& getIdentName(size_t ident_index) {
		return GlobalIdentTable.getName(ident_index);
	}
	inline const string& getIdentName(const AST::Element &elt) {
		assert(!elt.isIdentType(I_Void));
		return getIdentName(elt.getIndex());
	}

	// DyVarb

	inline void setDyVarbData(size_t ident_index, Object *data) {
		assert(GlobalIdentTable.at(ident_index).type == I_DyVarb);
		IdentTableUnit &itu = GlobalIdentTable.at(ident_index);
		if (itu.restype == T_Vary || itu.restype == data->type) {
			itu.DyVarb = data;
		}
		else {
			println("Error in set DyVarb for different type.");
		}
	}
	inline void setDyVarbResType(size_t ident_index, TypeUnit type) {
		assert(GlobalIdentTable.at(ident_index).type == I_DyVarb);
		IdentTableUnit &itu = GlobalIdentTable.at(ident_index);
		itu.restype = type;
	}
	inline Object* getDyVarbData(size_t ident_index) {
		assert(GlobalIdentTable.at(ident_index).type == I_DyVarb);
		return GlobalIdentTable.at(ident_index).DyVarb;
	}
	inline Object* getDyVarbData(const AST::Element &elt) {
		assert(elt.isIdentType(I_DyVarb));
		return getDyVarbData(elt.getIndex());
	}

	inline void setDyVarbRestType(size_t ident_index, TypeUnit type) {
		assert(GlobalIdentTable.at(ident_index).type == I_DyVarb);
		IdentTableUnit &itu = GlobalIdentTable.at(ident_index);
		itu.restype = type;
	}

	// Function

	inline FuncTableUnit& getFunction(const AST::Element &elt) {
		assert(elt.isIdentType(I_Function));
		return GlobalFunctionTable[GlobalIdentTable.at(elt.getIndex()).FunctionIndex];
	}

	// Data

	inline void setConstData(size_t ident_index, Object *data) {
		assert(GlobalIdentTable.at(ident_index).type == I_Data);
		GlobalIdentTable.at(ident_index).Data = data;
	}
	inline Object* getConstData(size_t ident_index) {
		assert(GlobalIdentTable.at(ident_index).type == I_Data);
		return GlobalIdentTable.at(ident_index).Data;
	}
	inline Object* getConstData(const AST::Element &elt) {
		assert(elt.isIdentType(I_Data));
		return getConstData(elt.getIndex());
	}

	inline TypeUnit getType(const AST::Element &elt) {
		assert(elt.isIdentType(I_Type));
		return GlobalIdentTable.at(elt.getIndex()).TypeIndex;
	}

	// Indet

	inline Object* getIdentData(const AST::Element &elt) {
		if (elt.isIdentType(I_DyVarb)) {
			return getDyVarbData(elt);
		}
		else if (elt.isIdentType(I_Data)) {
			return getConstData(elt);
		}
		else if (elt.isIdentType(I_Function)) {
			return new Objects::Function(getFunction(elt).getID());
		}
		else if (elt.isIdentType(I_Type)) {
			return new Objects::Type(GlobalIdentTable.at(elt.getIndex()).TypeIndex);
		}
		return nullptr;
	}
}
