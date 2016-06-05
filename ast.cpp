#include "ast.h"
#include <string>
using std::string;

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
	explicit ObjectData::ObjectData(const T &data) {
		setData(data);
	}
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
	template <typename T>
	void ObjectData::setData(const T & data) {
		if (!pointer) {
			pointer = new T(data);
			size = sizeof(T);
		}
		else {
			*getPointer<T>() = data;
		}
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
	void ASTNode::initialize(Function *fun = nullptr, Parameters *par = nullptr) {
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
	ASTNode* ASTNode::clone() const {
		ASTNode *copy = new ASTNode();
		copy->type = this->type;
		switch (this->type) {
		case AST_DATA:
			copy->objdata = this->objdata->clone();
			break;
		case AST_FUNC:
			copy->fundata.func = this->fundata.func; // TODO
			copy->fundata.pars = this->fundata.pars; // TODO
			break;
		default:
			;
		}
		return copy;
	}

	// ToString
	string to_string(const ObjectData *obj) {
		if (obj->pointer == nullptr)
			return "Null";
		/*System::charptr data(sizeof(void*) * 2 + 2);
		sprintf(data, "0x%p", pointer);
		return std::string(data);*/
		return std::to_string(*(int*)obj->pointer);
	}
	string to_string(const Function* func) {
		// Judge Null
		if (func == nullptr)
			return "Null";
		// Main
		string str;
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
	string to_string(const Parameters *pars) {
		// Judge Null
		if (pars == nullptr)
			return "Null";
		// Main
		string str;
		str.append("[PARS |");
		if (pars->list.empty()) {
			str.append(" NIL");
		}
		else {
			for (auto &l : pars->list) {
				str.append(" ");
				str.append(ICM::to_string(l));
			}
		}
		str.append("]");
		return str;
	}
	string to_string(const ASTNode *astn) {
		// Judge Null
		if (astn == nullptr)
			return "Null";
		// Main
		string str;
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
