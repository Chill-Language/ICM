#include "basic.h"
#include "tostring.h"
#include "objects.h"
#include "keyword.h"

namespace ICM
{
	// ToString
	string to_string(DefaultType type)
	{
		switch (type) {
		case T_Null:       return "Null";
		case T_Object:     return "Object";
		case T_Nil:        return "Nil";
		case T_Identifier: return "Identifier";
		case T_Keyword:    return "Keyword";
		case T_Number:     return "Number";
		case T_Boolean:    return "Boolean";
		case T_String:     return "String";
		case T_Symbol:     return "Symbol";
		case T_List:       return "List";
		case T_Disperse:   return "Disperse";
		case T_Vary:       return "Vary";
		case T_Function:   return "Function";
		case T_Type:       return "Type";
		default:           return "UnfoundType";
		}
	}
	string to_string(KeywordID key)
	{
		size_t id = DefKeywordTable.findValue(key);
		if (id != DefKeywordTable.size())
			return DefKeywordTable.getData(id).first;
		else
			return "UnfoundKeyword";
	}
	string to_string(const ObjectPtr &obj) {
		return obj.to_string();
	}
	string to_string(const DataList &list) {
		return Convert::to_string<'[', ']'>(list.begin(), list.end(), [](const ObjectPtr &obj) { return to_string(obj); });
	}
	string to_string(const AST *ast) {
		return to_string(*ast);
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
	// Function
	inline std::string to_string(const Function::Signature::List &list) {
		return Convert::to_string(list.begin(), list.end());
	}
	std::string to_string(const Function::Signature &sign) {
		return sign.to_string();
	}

	std::string to_string(const TypeObject &to) {
		return to.to_string();
	}
	std::string to_string(const shared_ptr<TypeObject> &top) {
		return top->to_string();
	}

	//=======================================
	// * AST
	//=======================================
	string to_string(const AST::Element &element) {
		if (element.isData())
			return "D(" + element.getData()->to_string() + ")";
		else
			return "R[" + std::to_string(element.getRefer()) + "]";
	}
	string to_string_code(const AST::Element &element) {
		if (element.isData())
			return element.getData()->to_string_code();
		else
			return "{" + std::to_string(element.getRefer()) + "}";
	}
	string to_string(const AST::Node &node) {
		string str = "<N(" + std::to_string(node.getIndex()) + "):";
		for (auto &e : node)
			str.append(" " + to_string(e));
		str.append(">");
		return str;
	}
	string to_string_code(const AST::Node &node) {
		string str("[" + std::to_string(node.getIndex()) + "]: " + "(" + to_string_for_order(node) + ")");
		return str;
	}
	string to_string_for_order(const AST::Node &node) {
		string str;
		for (auto &e : node)
			str.append(to_string_code(e) + " ");
		if (!node.empty())
			str.pop_back();
		return str;
	}
	string to_string(const AST &ast) {
		string str("{AST:");
		if (ast.empty())
			str.append("NIL");
		else {
			for (const auto &e : ast.getTableRange()) {
				str.append("\n ");
				str.append(ICM::to_string(**e));
			}
		}
		str.append("\n}");
		return str;
	}
	string to_string_code(const AST &ast) {
		string str("{AST:");
		if (ast.empty())
			str.append("NIL");
		else {
			for (const auto &e : ast.getTableRange()) {
				str.append("\n ");
				str.append(ICM::to_string_code(**e));
			}
		}
		str.append("\n}");
		return str;
	}
}
