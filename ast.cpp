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
	Parameters* Parameters::clone() const {
		Parameters *cpy = new Parameters();
		for (auto &l : this->list)
			cpy->list.push_back(l->clone());
		return cpy;
	}
	// Deep Copy
	Parameters* Parameters::deep_clone() const {
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
			initialize(new ObjectData());
			break;
		case AST_FUNC:
			initialize(new Function(), new Parameters());
			break;
		default:
			this->type = type;
		}
	}
	void ASTNode::initialize(ObjectData *dat) {
		this->type = AST_DATA;
		this->objdata = dat;
	}
	void ASTNode::initialize(Function *fun, Parameters *par) {
		this->type = AST_FUNC;
		this->fundata.func = fun;
		this->fundata.pars = par;
	}
	void ASTNode::release() {
		if (reference)
			return;
		switch (type) {
		case AST_DATA:
			delete this->objdata;
			break;
		case AST_FUNC:
			delete this->fundata.func;
			delete this->fundata.pars;
			break;
		default:
			;
		}
	}
	// Shallow Copy
	ASTNode* ASTNode::clone() const {
		ASTNode *copy = new ASTNode();
		copy->type = this->type;
		switch (this->type) {
		case AST_DATA:
			copy->objdata = this->objdata->clone();
			break;
		case AST_FUNC:
			copy->fundata.func = this->fundata.func;
			copy->fundata.pars = this->fundata.pars; /* Shallow */
			break;
		default:
			;
		}
		return copy;
	}
	// Deep Copy
	ASTNode* ASTNode::deep_clone() const {
		ASTNode *copy = new ASTNode();
		copy->type = this->type;
		switch (this->type) {
		case AST_DATA:
			copy->objdata = this->objdata->clone();
			break;
		case AST_FUNC:
			copy->fundata.func = this->fundata.func;
			copy->fundata.pars = this->fundata.pars->deep_clone(); /* Deep */
			break;
		default:
			;
		}
		return copy;
	}
	// AST
	AST* AST::pushNode(ASTNodeType type) {
		if (root == nullptr) {
			root = new ASTNode(type);
			currptr = root;
			farthptrs.push(currptr);
		}
		else {
			ASTNode *tmp = new ASTNode(type);
			if (currptr->type == AST_FUNC)
				currptr->pushpars(tmp);
			else
				farthptrs.top()->pushpars(tmp);
			currptr = tmp;
			if (tmp->type == AST_FUNC)
				farthptrs.push(currptr);
		}
		return this;
	}
	int AST::retNode() {
		if (currptr != root) {
			currptr = farthptrs.top();
			farthptrs.pop();
			return 0;
		}
		else {
			return -1; // Error
		}
	}
}
