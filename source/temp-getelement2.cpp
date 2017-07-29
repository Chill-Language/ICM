#include "basic.h"
#include "instruction.h"
#include "temp-getelement.h"
#include "runtime/objectdef.h"

namespace ICM
{
	Object getLiteral(const Instruction::Element & elt, const Compiler::ElementPool &EP) {
		return getLiteral(*(ASTBase::Element*)&elt, EP);
	}
	Object * getConstData(const Instruction::Element & elt) {
		assert(elt.isIdentType(I_Data));
		return getConstData(elt.getIdentIndex());
	}
	TypeUnit getType(const Instruction::Element & elt) {
		assert(elt.isIdentType(I_Type));
		return getFromIdentTable(elt.getIdentIndex()).TypeIndex;
	}
	FuncTableUnit & getStFunc(const Instruction::Element & elt) {
		assert(elt.isIdentType(I_StFunc));
		return GlobalFunctionTable[getFromIdentTable(elt.getIdentIndex()).FunctionIndex];
	}
	Object * getDyVarbData(const Instruction::Element & elt) {
		assert(elt.isIdentType(I_DyVarb));
		return getDyVarbData(elt.getIdentIndex());
	}
	const string& getIdentName(const Instruction::Element &elt) {
		assert(!elt.isIdentType(I_Void));
		return getIdentName(elt.getIdentIndex());
	}
	Object* getIdentData(const Instruction::Element &elt) {
		if (elt.isIdentType(I_DyVarb)) {
			return getDyVarbData(elt);
		}
		else if (elt.isIdentType(I_Data)) {
			return getConstData(elt);
		}
		else if (elt.isIdentType(I_StFunc)) {
			return new Objects::Function(getStFunc(elt).getID());
		}
		else if (elt.isIdentType(I_Type)) {
			return new Objects::Type(getFromIdentTable(elt.getIdentIndex()).TypeIndex);
		}
		return nullptr;
	}
}
