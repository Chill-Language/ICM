#include "ast.h"

// The Different of 'NIL' & 'Null' :
//   'NIL' is a real value for empty list or blank name. It's Allowed.
//   'Null' is a reference for nullptr. It's Not Allowed.
//  Example:
//    println(*(new Parameters()));    // Shows 'NIL'.
//    println(*(Parameters*)nullptr);  // Shows 'Null'.

namespace ICM
{
	// Method
	// Parameters
	// Shallow Copy  /* TODO : If it can be deleted. */
	ASTNode::Parameters* ASTNode::Parameters::clone() const {
		Parameters *cpy = new Parameters();
		for (auto &l : this->list)
			cpy->list.push_back(l->clone());
		return cpy;
	}
	// Deep Copy
	ASTNode::Parameters* ASTNode::Parameters::deep_clone() const {
		Parameters *cpy = new Parameters();
		for (auto &l : this->list)
			cpy->list.push_back(l->deep_clone());
		return cpy;
	}
	// ASTNode
	void ASTNode::initialize(ASTNodeType type) {
		this->reference = false;
		switch (type) {
		case AST_DATA:
			initialize(ObjectPtr());
			break;
		case AST_NODE:
			initialize(new Function(), new Parameters());
			break;
		default:
			this->type = type;
		}
	}
	void ASTNode::initialize(const ObjectPtr &dat) {
		this->type = AST_DATA;
		this->objdata = dat;
	}
	void ASTNode::initialize(Function *fun, Parameters *par) {
		this->type = AST_NODE;
		this->fundata.func = fun;
		this->fundata.pars = par;
	}
	void ASTNode::release() {
		if (reference)
			return;
		switch (type) {
		case AST_DATA:
			break;
		case AST_NODE:
			delete this->fundata.func;
			delete this->fundata.pars;
			break;
		default:
			break;
		}
	}
	// Shallow Copy
	ASTNode* ASTNode::clone() const {
		ASTNode *copy = new ASTNode();
		copy->type = this->type;
		switch (this->type) {
		case AST_DATA:
			copy->objdata = ObjectPtr(this->objdata->clone());
			break;
		case AST_NODE:
			copy->fundata.func = this->fundata.func;
			copy->fundata.pars = this->fundata.pars; /* Shallow */
			break;
		default:
			break;
		}
		return copy;
	}
	// Deep Copy
	ASTNode* ASTNode::deep_clone() const {
		ASTNode *copy = new ASTNode();
		copy->type = this->type;
		switch (this->type) {
		case AST_DATA:
			copy->objdata = ObjectPtr(this->objdata->clone());
			break;
		case AST_NODE:
			copy->fundata.func = this->fundata.func;
			copy->fundata.pars = this->fundata.pars->deep_clone(); /* Deep */
			break;
		default:
			break;
		}
		return copy;
	}

	// AST
	void AST::pushNode() {
		if (root == nullptr) {
			root = new ASTNode(AST_NODE);
			currptr = root;
			farthptrs.push(currptr);
		}
		else {
			ASTNode *tmp = new ASTNode(AST_NODE);
			currptr->pushpars(tmp);
			farthptrs.push(currptr);
			currptr = tmp;
		}
	}
	int AST::retNode() {
		if (!farthptrs.empty()) {
			currptr = farthptrs.top();
			farthptrs.pop();
			return 0;
		}
		else {
			return -1; // Error
		}
	}
	void AST::pushData(const ObjectPtr &op) {
		if (root == nullptr)
			pushNode();
		ASTNode *tmp = new ASTNode(AST_DATA);
		tmp->setdata(op);
		currptr->pushpars(tmp);
	}
	void AST::setFunc(FuncType type, FuncID id) {
		currptr->setfunc(type, id);
	}
}
