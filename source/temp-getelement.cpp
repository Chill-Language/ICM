#include "basic.h"
#include "parser/ast.h"
#include "parser/literal.h"
#include "temp-getelement.h"

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
	const string & getIdentName(const ASTBase::Element & elt) {
		IdentKey key = elt.isIdentType(I_Void) ? elt.getIndex() : getKeyFromIdentTable(getIdentID(elt));
		return Compiler::GlobalIdentNameMap.getKey(key);
	}
	const string& getIdentName(size_t ident_index) {
		return Compiler::GlobalIdentNameMap.getKey(ident_index);
	}
	const string& getIdentName(const IdentIndex &ident_index) {
		IdentKey key = getKeyFromIdentTable(ident_index);
		return Compiler::GlobalIdentNameMap.getKey(key);
	}

	bool issame(const IdentIndex &t1, const IdentIndex &t2) {
		return (t1.ident_index == t2.ident_index) && (t1.space_index == t2.space_index);
	}

	const IdentIndex& getIdentID(const ASTBase::Element & elt) {
		assert(!elt.isIdentType(I_Void));
		// TODO : It causes unknown change to elt.
		return reinterpret_cast<const IdentIndex&>(elt.getIndex());
	}
	IdentIndex getGlobalFunctionIdentIndex(const string & name) {
		return { 0, findFromIdentTable(0, Compiler::GlobalIdentNameMap[name]) };
	}
	IdentIndex insertGlobalDyVarbIdentIndex(const string & name) {
		return { 0, insertFromIdentTable(0, Compiler::GlobalIdentNameMap[name], I_DyVarb) };
	}
}
