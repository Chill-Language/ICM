#pragma once
#include "object.h"
#include "identtable.h"

namespace ICM
{
	namespace ASTBase { struct Element; }
	
	Object getLiteral(const ASTBase::Element &elt);

	//bool getCompiletimeFunc(const ASTBase::Element &elt, FuncTableUnit &ftu);
	bool getCompiletimeData(const ASTBase::Element &elt, Object &value);

	const IdentIndex& getIdentID(const ASTBase::Element &elt);
	const string& getIdentName(const ASTBase::Element &elt);
}
