#pragma once
#include "object.h"
#include "tabledata.h"

namespace ICM
{
	namespace Instruction { struct Element; }

	Object getLiteral(const Instruction::Element & elt);
	inline Object* createObjectFromLiteral(const Instruction::Element &elt) {
		return new Object(getLiteral(elt));
	}
	const string& getIdentName(size_t ident_index);
	const string& getIdentName(const IdentIndex &ident_index);
	const string& getIdentName(const Instruction::Element &elt);
	// DyVarb

	inline void setDyVarbData(const IdentIndex &ident_index, Object *data) {
		assert(getFromIdentTable(ident_index).type == I_DyVarb);
		IdentTableUnit &itu = getFromIdentTable(ident_index);
		if (itu.restype == T_Vary || itu.restype == data->type) {
			itu.DyVarb = data;
		}
		else {
			println("Error in set DyVarb for different type.");
		}
	}
	inline void setDyVarbResType(const IdentIndex &ident_index, TypeUnit type) {
		assert(getFromIdentTable(ident_index).type == I_DyVarb);
		IdentTableUnit &itu = getFromIdentTable(ident_index);
		itu.restype = type;
	}
	inline Object* getDyVarbData(const IdentIndex &ident_index) {
		assert(getFromIdentTable(ident_index).type == I_DyVarb);
		return getFromIdentTable(ident_index).DyVarb;
	}
	Object* getDyVarbData(const Instruction::Element &elt);

	inline void setDyVarbRestType(const IdentIndex &ident_index, TypeUnit type) {
		assert(getFromIdentTable(ident_index).type == I_DyVarb);
		IdentTableUnit &itu = getFromIdentTable(ident_index);
		itu.restype = type;
	}
	FuncTableUnit& getStFunc(const Instruction::Element &elt);

	// Data

	inline void setConstData(const IdentIndex &ident_index, Object *data) {
		assert(getFromIdentTable(ident_index).type == I_Data);
		getFromIdentTable(ident_index).Data = data;
	}
	inline Object* getConstData(const IdentIndex &ident_index) {
		assert(getFromIdentTable(ident_index).type == I_Data);
		return getFromIdentTable(ident_index).Data;
	}
	TypeUnit getType(const Instruction::Element &elt);
	Object* getIdentData(const Instruction::Element &elt);
}
