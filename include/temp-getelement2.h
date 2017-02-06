#pragma once
//#pragma message("===Compiling temp-getelement2.h===")
#include "object.h"
#include "tabledata.h"

namespace ICM
{
	namespace Instruction { struct Element; }

	Object getLiteral(const Instruction::Element & elt);
	inline Object* createObjectFromLiteral(const Instruction::Element &elt) {
		return new Object(getLiteral(elt));
	}
	inline const string& getIdentName(size_t ident_index) {
		return GlobalIdentTable.getName(ident_index);
	}
	const string& getIdentName(const Instruction::Element &elt);
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
	Object* getDyVarbData(const Instruction::Element &elt);

	inline void setDyVarbRestType(size_t ident_index, TypeUnit type) {
		assert(GlobalIdentTable.at(ident_index).type == I_DyVarb);
		IdentTableUnit &itu = GlobalIdentTable.at(ident_index);
		itu.restype = type;
	}
	FuncTableUnit& getFunction(const Instruction::Element &elt);

	// Data

	inline void setConstData(size_t ident_index, Object *data) {
		assert(GlobalIdentTable.at(ident_index).type == I_Data);
		GlobalIdentTable.at(ident_index).Data = data;
	}
	inline Object* getConstData(size_t ident_index) {
		assert(GlobalIdentTable.at(ident_index).type == I_Data);
		return GlobalIdentTable.at(ident_index).Data;
	}
	TypeUnit getType(const Instruction::Element &elt);
	Object* getIdentData(const Instruction::Element &elt);
}
