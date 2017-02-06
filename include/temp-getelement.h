#pragma once
//#pragma message("===Compiling temp-getelement.h===")
#include "object.h"

namespace ICM
{
	namespace ASTBase { struct Element; }
	
	Object getLiteral(const ASTBase::Element &elt);
	void setIdent(ASTBase::Element &elt, IdentType type, size_t index);

	size_t getIdentID(const ASTBase::Element &elt);
	const string& getIdentName(const ASTBase::Element &elt);
}
