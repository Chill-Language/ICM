#include "basic.h"
#include "tostring.h"
#include "objects.h"
using std::string;
using std::to_string;
#include <algorithm>

namespace ICM
{
	// ToString
	string to_string(DefaultType type)
	{
		switch (type) {
		case T_Null:       return "Null";
		case T_Nil:        return "Nil";
		case T_LBracket:   return "LBracket";
		case T_RBracket:   return "RBracket";
		case T_LSBracket:  return "LSBracket";
		case T_RSBracket:  return "RSBracket";
		case T_Number:     return "Number";
		case T_Boolean:    return "Boolean";
		case T_Identifier: return "Identifier";
		case T_String:     return "String";
		case T_Symbol:     return "Symbol";
		case T_List:       return "List";
		case T_Comment:    return "Comment";
		default:           return "";
		}
	}
	string to_string(const ObjectPtr &obj) {
		if (obj == nullptr)
			return "Null";
		return obj->to_string();
	}
	string to_string(const DataList &list) {
		return Convert::to_string(list.begin(), list.end(), [](const ObjectPtr &obj) { return to_string(obj); });
	}
	string to_string(const ASTNode::Function* func) {
		// Judge Null
		if (func == nullptr)
			return "Null";
		// Main
		string str;
		str.append("F(");
		if (func->type != FUNC_NIL) {
			str.append(std::to_string(func->type));
			str.append(",");
			str.append(std::to_string(func->id));
		}
		else {
			str.append("NIL");
		}
		str.append(")");

		return str;
	}
	string to_string(const ASTNode::Parameters *pars) {
		// Judge Null
		if (pars == nullptr)
			return "Null";
		// Main
		string str;
		str.append("[P:");
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
			str.append("<AST: NIL>");
		}
		else if (astn->type == AST_DATA) {
			str.append("D(");
			if (astn->objdata)
				str.append(to_string(astn->objdata));
			else
				str.append("NIL");
			str.append(")");
		}
		else if (astn->type == AST_NODE) {
			str.append("N< ");
			if (astn->fundata.func)
				str.append(to_string(astn->fundata.func));
			else
				str.append("NIL");
			str.append(", ");
			if (astn->fundata.pars)
				str.append(to_string(astn->fundata.pars));
			else
				str.append("NIL");
			str.append(" >");
		}
		else {
			str.append("ASTNodeTypeError");
		}
		return str;
	}
	string to_string(const AST *ast) {
		return string("{AST: ") + to_string(ast->root) + string("}");
	}
	// MatchResult
	string to_string(const MatchResult *mr) {
		// Judge Null
		if (mr == nullptr)
			return "Null";
		// Main
		string str;
		str.append("(");
		str.append(to_string(mr->getType()));
		str.append(", \'");
		str.append(mr->getString());
		str.append("')");
		return str;
	}
}
