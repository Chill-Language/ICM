#include "ast.h"
#include <cstring>
#include <string>
using std::to_string; // Not Find 'std::to_string' in GCC, So Delete 'using std::string'.

// The Different of 'NIL' & 'Null' :
//   'NIL' is a real value for empty list or blank name. It's Allowed.
//   'Null' is a reference for nullptr. It's Not Allowed.
//  Example:
//    println(*(new Parameters()));    // Shows 'NIL'.
//    println(*(Parameters*)nullptr);  // Shows 'Null'.


namespace ICM
{
	// Method
	// ObjectData
	template <typename T>
	void ObjectData::release() {
		getPointer<T>()->~T();
		pointer = nullptr;
		size = 0;
	}
	ObjectData* ObjectData::clone() const {
		ObjectData *cpy = new ObjectData(*this);
		if (this->size) {
			cpy->pointer = memcpy((char*)malloc(this->size), this->pointer, this->size);
		}
		return cpy;
	}
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

	// ToString
	std::string to_string(const ObjectData *obj) {
		if (obj->pointer == nullptr)
			return "Null";
		/*System::charptr data(sizeof(void*) * 2 + 2);
		sprintf(data, "0x%p", pointer);
		return std::string(data);*/
		return std::to_string(*(int*)obj->pointer);
	}
	std::string to_string(const Function* func) {
		// Judge Null
		if (func == nullptr)
			return "Null";
		// Main
		std::string str;
		str.append("(FUNC | ");
		if (func->type != FUNC_NIL) {
			str.append("(");
			str.append(std::to_string(func->type));
			str.append(",");
			str.append(std::to_string(func->id));
			str.append(")");
		}
		else {
			str.append("NIL");
		}
		str.append(")");

		return str;
	}
	std::string to_string(const Parameters *pars) {
		// Judge Null
		if (pars == nullptr)
			return "Null";
		// Main
		std::string str;
		str.append("[PARS |");
		if (pars->list.empty()) {
			str.append(" NIL");
		}
		else {
			for (auto &l : pars->list) {
				str.append(" ");
				str.append(to_string(l));
			}
		}
		str.append("]");
		return str;
	}
	std::string to_string(const ASTNode *astn) {
		// Judge Null
		if (astn == nullptr)
			return "Null";
		// Main
		std::string str;
		if (astn->type == AST_NIL) {
			str.append("<AST | NIL>");
		}
		else if (astn->type == AST_DATA) {
			str.append("<AST:Data | ");
			if (astn->objdata)
				str.append(ICM::to_string(astn->objdata));
			else
				str.append("NIL");
			str.append(">");
		}
		else if (astn->type == AST_FUNC) {
			str.append("<AST:Node | ");
			if (astn->fundata.func)
				str.append(to_string(astn->fundata.func));
			else
				str.append("NIL");
			str.append(", ");
			if (astn->fundata.pars)
				str.append(to_string(astn->fundata.pars));
			else
				str.append("NIL");
			str.append(">");
		}
		else {
			str.append("ASTNodeTypeError");
		}
		return str;
	}
}
