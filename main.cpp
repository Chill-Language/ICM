#include "prints.h"
#include "charptr.h"
#include "ast.h"
using std::string;
using std::vector;
using namespace System;

template <typename T>
void println(const T &obj)
{
	Output::println(obj.to_string());
}


namespace ICM
{
	void ASTNode::initialize(ASTNodeType type) {
		this->type = type;
		switch (type) {
		case AST_DATA:
			break;
		case AST_FUNC:
			this->func = new Function();
			this->pars = new Parameters();
			break;
		}
	}
	string ASTNode::to_string() const {
		string str;
		if (type == AST_DATA) {
			str.append("<ASTData | ");
			str.append(data->to_string());
			str.append(">");
		}
		else if (type == AST_FUNC) {
			str.append("<ASTNode | ");
			if (func)
				str.append(func->to_string());
			else
				str.append("NIL");
			str.append(", ");
			if (pars)
				str.append(pars->to_string());
			else
				str.append("NIL");
			str.append(">");
		}
		else {
			str.append("ASTNodeTypeError");
		}
		return str;
	}
	string Function::to_string() const {
		string str;
		str.append("(FUNC | ");
		if (this)
			str.append(std::to_string(this->id));
		else
			str.append("NIL");
		str.append(")");
		return str;
	}
	string Parameters::to_string() const {
		string str;
		str.append("[PARS |");
		if (this) {
			for (auto &l : list) {
				str.append(" ");
				str.append(l->to_string());
			}
		}
		else {
			str.append(" NIL");
		}
		str.append("]");
		return str;
	}
}

using namespace ICM;

int main(void)
{
	// (+ 5 6)
	AST ast;
	ASTNode *root;
	ObjectData *dat1, *dat2;
	ASTNode *adat1, *adat2;

	root = new ASTNode();
	root->initialize(AST_FUNC);

	root->setfunc(FUNC_DEF, 1);
	dat1 = new ObjectData(5);
	dat2 = new ObjectData(6);
	adat1 = new ASTNode(dat1);
	adat2 = new ASTNode(dat2);

	root->pushpars(adat1);
	root->pushpars(adat2);

	println(*root);
	

	return 0;
}
