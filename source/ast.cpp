#include "ast.h"

namespace ICM
{
	//=======================================
	// * Class AST
	//=======================================
	void AST::pushData(Objects::Object *op) {
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
