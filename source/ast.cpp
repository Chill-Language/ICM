#include "basic.h"
#include "ast.h"

namespace ICM
{
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
