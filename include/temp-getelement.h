#pragma once
#include "runtime/object.h"
#include "identtable.h"
#include "parser/literal.h"

namespace ICM
{
	namespace ASTBase { struct Element; }
	
	Object getLiteral(const ASTBase::Element &elt, const Compiler::ElementPool &EP);

	//bool getCompiletimeFunc(const ASTBase::Element &elt, FuncTableUnit &ftu);
	bool getCompiletimeData(const ASTBase::Element &elt, Object &value);

	const IdentIndex& getIdentID(const ASTBase::Element &elt);
	const string& getIdentName(const ASTBase::Element &elt);

	IdentIndex getGlobalFunctionIdentIndex(const string &name);
	IdentIndex insertGlobalDyVarbIdentIndex(const string & name);
}
