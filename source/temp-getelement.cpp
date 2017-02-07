#include "basic.h"
#include "ast.h"
#include "objectdef.h"
#include "instruction.h"
#include "temp-getelement.h"
#include "temp-getelement2.h"

namespace ICM
{
	Object getLiteral(const ASTBase::Element &elt) {
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
	Object getLiteral(const Instruction::Element & elt) {
		return getLiteral(*(ASTBase::Element*)&elt);
	}
	const string & getIdentName(const ASTBase::Element & elt) {
		IdentKey key = elt.isIdentType(I_Void) ? elt.getIndex() : GlobalIdentTable.getKey(elt.getIndex());
		return Compiler::GlobalIdentNameMap.getKey(key);
	}
	const string& getIdentName(size_t ident_index) {
		IdentKey key = GlobalIdentTable.getKey(ident_index);
		return Compiler::GlobalIdentNameMap.getKey(key);
	}
	void setIdent(ASTBase::Element & elt, IdentType type, size_t index) {
		elt = ASTBase::Element::Identifier(type, index);
	}
	size_t getIdentID(const ASTBase::Element & elt) {
		assert(!elt.isIdentType(I_Void));
		return elt.getIndex();
	}
	Object * getConstData(const Instruction::Element & elt) {
		assert(elt.isIdentType(I_Data));
		return getConstData(elt.getIndex());
	}
	TypeUnit getType(const Instruction::Element & elt) {
		assert(elt.isIdentType(I_Type));
		return GlobalIdentTable.at(elt.getIndex()).TypeIndex;
	}
	FuncTableUnit & getFunction(const Instruction::Element & elt) {
		assert(elt.isIdentType(I_Function));
		return GlobalFunctionTable[GlobalIdentTable.at(elt.getIndex()).FunctionIndex];
	}
	Object * getDyVarbData(const Instruction::Element & elt) {
		assert(elt.isIdentType(I_DyVarb));
		return getDyVarbData(elt.getIndex());
	}
	const string& getIdentName(const Instruction::Element &elt) {
		assert(!elt.isIdentType(I_Void));
		return getIdentName(elt.getIndex());
	}
	Object* getIdentData(const Instruction::Element &elt) {
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
