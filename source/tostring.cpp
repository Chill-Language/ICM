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
		switch (key)
		{
		case KeywordID::IF:       return "if";
		case KeywordID::THEN:     return "then";
		case KeywordID::ELSE:     return "else";
		case KeywordID::ELSIF:    return "elsif";
		case KeywordID::FOR:      return "for";
		case KeywordID::IN:       return "in";
		case KeywordID::WHILE:    return "while";
		case KeywordID::LOOP:     return "loop";
		case KeywordID::BREAK:    return "break";
		case KeywordID::CASE:     return "case";
		case KeywordID::WHEN:     return "when";
		case KeywordID::FUNCTION: return "function";
		default:                  return "UnfoundKeyword";
		}
	}
	string to_string(const ObjectPtr &obj) {
		return obj.to_string();
	}
	string to_string(const DataList &list) {
		return Convert::to_string<'[', ']'>(list.begin(), list.end(), [](const ObjectPtr &obj) { return to_string(obj); });
	}
	string to_string(const AST *ast) {
		return ast->to_string();
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
}
