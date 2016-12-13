#include "basic.h"
#include "ast.h"
#include "object.h"
#include "parser.h"
#include "literal.h"

namespace ICM
{
	//=======================================
	// * Struct Element
	//=======================================
	namespace ASTBase
	{
		Element Element::Data(size_t type, size_t index) {
			assert(type == T_Boolean || type == T_Number || type == T_String || type == T_Nil);
			Element r(E_Data);
			r.setSubType(type);
			r.data.index = index;
			return r;
		}
		Element Element::Refer(size_t index) {
			return Element(E_Refer).setIndex(index);
		}
		Element Element::Keyword(Keyword::KeywordID key) {
			return Element(E_Key, key);
		}
		Element Element::Identifier(const string & name) {
			return Element(E_Ident).setIndex(GlobalIdentifierMap[name]);
		}
		// TODO
		Element Element::Variable(size_t index) {
			return Element(E_Ident, I_Variable).setIndex(index);
		}
		Element Element::Function(size_t index) {
			return Element(E_Ident, I_Function).setIndex(index);
		}

		size_t Element::getRefer() const {
			assert(isRefer());
			return data.index;
		}
		bool Element::getBoolean() const {
			assert(isBoolean());
			return data.bvalue;
		}
		Keyword::KeywordID Element::getKeyword() const {
			assert(isKeyword());
			return (Keyword::KeywordID)getSubType();
		}
	}
	//=======================================
	// * Class AST
	//=======================================
	void AST::pushData(Element&& element) {
		currptr->push_back(element);
	}
	void AST::pushNode() {
		currptr->push_back(Element::Refer(currindex));
		NodePtr tmp(new Node(currindex++));
		farthptrs.push(currptr);
		currptr = tmp.get();
		Table.push_back(std::move(tmp));
	}
	bool AST::retNode() {
		if (!farthptrs.empty()) {
			currptr = farthptrs.top();
			farthptrs.pop();
			return true;
		}
		return false;
	}
}
