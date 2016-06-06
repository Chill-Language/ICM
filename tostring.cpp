#include <string>
#include "charptr.h"
#include "ast.h"
using std::string;
using std::to_string;

namespace ICM
{
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
				str.append(to_string(l));
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
	string to_string(const AST *ast) {
		return string("{AST | ") + to_string(ast->root) + string("}");
	}
}