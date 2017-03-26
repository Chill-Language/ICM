#include "basic.h"
#include "ast.h"
#include "objectdef.h"
#include "instruction.h"
#include "temp-getelement.h"
#include "temp-getelement2.h"

namespace ICM
{

	bool getCompiletimeData(const ASTBase::Element &elt, Object &value) {
		if (elt.isLiteral()) {
			value = getLiteral(elt);
			return true;
		}
		else if (elt.isIdent()) {
			if (elt.isIdentType(I_StFunc)) {
				//value = getStFunc(elt);
				return true;
			}
		}
		return false;
	}

	bool getCompiletimeFunc(const ASTBase::Element &elt, FuncTableUnit &ftu) {
		//getStFunc(elt);
		return true;
	}

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
		IdentKey key = elt.isIdentType(I_Void) ? elt.getIndex() : getKeyFromIdentTable(elt.getIdentIndex());
		return Compiler::GlobalIdentNameMap.getKey(key);
	}
	const string& getIdentName(size_t ident_index) {
		return Compiler::GlobalIdentNameMap.getKey(ident_index);
	}
	const string& getIdentName(const IdentIndex &ident_index) {
		IdentKey key = getKeyFromIdentTable(ident_index);
		return Compiler::GlobalIdentNameMap.getKey(key);
	}
	const IdentIndex& getIdentID(const ASTBase::Element & elt) {
		assert(!elt.isIdentType(I_Void));
		return elt.getIdentIndex();
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
	IdentIndex getGlobalFunctionIdentIndex(const string & name) {
		return { 0, findFromIdentTable(0, Compiler::GlobalIdentNameMap[name]) };
	}
	IdentIndex insertGlobalDyVarbIdentIndex(const string & name) {
		return { 0, insertFromIdentTable(0, Compiler::GlobalIdentNameMap[name], I_DyVarb) };
	}
}
