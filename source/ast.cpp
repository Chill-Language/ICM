#include "ast.h"
#include "object.h"

namespace ICM
{
	ASTBase::Element ASTBase::Element::Data(Object *op) {
		Element r(E_Data);
		r.setData(op);
		return r;
	}
	ASTBase::Element ASTBase::Element::Refer(size_t index) {
		Element r(E_Refer);
		r.data.id = index;
		return r;
	}
	void ASTBase::Element::setData(Object* op) {
		data.op = new ObjectPtr(op);
	}
	//=======================================
	// * Class AST
	//=======================================
	void AST::pushData(Object *op) {
		currptr->push_back(Element::Data(op));
	}
	void AST::pushNode() {
		currptr->push_back(Element::Refer(currindex));
		NodePtr tmp(new Node(currindex++));
		farthptrs.push(currptr);
		currptr = tmp.get();
		table.push_back(tmp);
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
