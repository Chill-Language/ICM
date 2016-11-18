#include "basic.h"
#include "object.h"
#include "keyword.h"
#include "tabledata.h"
#include "parser.h"
#include "instruction.h"

namespace ICM
{
	// ToString
	string to_string(DefaultType type)
	{
		if (TypeInfoTable.find(type) != TypeInfoTable.end())
			return TypeInfoTable[type].name;
		else {
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
	}
	string to_string(Parser::MatchType type)
	{
		using namespace Parser;

		switch (type) {
		case MT_Null:       return "Null";
		case MT_LBracket:   return "LBracket";
		case MT_RBracket:   return "RBracket";
		case MT_LSBracket:  return "LSBracket";
		case MT_RSBracket:  return "RSBracket";
		case MT_Comment:    return "Comment";
		case MT_Identifier: return "Identifier";
		case MT_Keyword:    return "Keyword";
		case MT_Number:     return "Number";
		case MT_Boolean:    return "Boolean";
		case MT_String:     return "String";
		case MT_Symbol:     return "Symbol";
		default:            return "UnfoundType";
		}
	}
	string to_string(Keyword::KeywordID key)
	{
		size_t id = GlobalKeywordTable.findValue(key);
		if (id != GlobalKeywordTable.size())
			return GlobalKeywordTable.getData(id).first;
		else
			return "UnfoundKeyword";
	}
	string to_string(Instruction::Instruction inst)
	{
		size_t id = Instruction::InstructionName.findKey(inst);
		if (id != Instruction::InstructionName.size())
			return Instruction::InstructionName.getValue(id);
		else
			return "UnknowInstruction";
	}
	string to_string(const ObjectPtr &obj) {
		return obj.to_string();
	}
	string to_string(const DataList &list) {
		return Convert::to_string<'[', ']'>(list.begin(), list.end(), [](const ObjectPtr &obj) { return to_string(obj); });
	}
	string to_string(const vector<ObjectPtr> &list) {
		return Convert::to_string<'[', ']'>(list.begin(), list.end(), [](const ObjectPtr &obj) { return to_string(obj); });
	}
	string to_string(const vector<Object*> &list) {
		return Convert::to_string<'[', ']'>(list.begin(), list.end(), [](const Object* obj) { return obj->to_string(); });
	}
	// MatchResult
	string to_string(const Parser::MatchResult &mr) {
		// Main
		string str;
		str.append("(");
		str.append(ICM::to_string(mr.getType()));
		str.append(", \'");
		str.append(mr.getString());
		str.append("')");
		return str;
	}
	// Function
	std::string to_string(const Function::Signature::List &list) {
		return Convert::to_string(list.begin(), list.end());
	}

	string to_string(const Function::Signature &sign) {
		std::string str;

		const auto &its = sign.getInType();
		const auto &ots = sign.getOutType();
		if (!its.empty()) {
			if (its.size() != 1) str.push_back('(');
			str.append(Convert::to_string(its.begin(), its.end()));
			if (sign.isLastArgs()) str.push_back('*');
			if (its.size() != 1) str.push_back(')');
		}
		else {
			str.append("Void");
		}
		str.append(" -> ");
		str.append(to_string(ots));

		return str;
	}
	string to_string(const TypeObject &to) {
		string str(ICM::to_string(to.getType()));
		if (to.isFunc()) {
			str.append("(" + ICM::to_string(to.getSign()) + ")");
		}
		return str;
	}
	string to_string(const TypeBase::FunctionType& ft)
	{
		return TypeBase::to_string(ft);
	}
	string to_string(const TypeBase::VariableType& vt)
	{
		return TypeBase::to_string(vt);
	}
	//=======================================
	// * AST
	//=======================================
	string to_string(const AST::Element &element) {
		if (element.isData())
			return "D(" + element.getData().to_string_code() + ")";
		else if (element.isRefer())
			return "R{" + std::to_string(element.getRefer()) + "}";
		else if (element.isDispData())
			return "Dp(" + element.getIdentifier() + ")";
		else if (element.isDispRefer())
			return "Dp{" + std::to_string(element.getRefer()) + "}";
		else if (element.isKeyword())
			return "K(" + ICM::to_string(element.getKeyword()) + ")";
		else if (element.isVariable())
			return "V(" + element.getVariable().getName() + ")";
		else if (element.isFunction())
			return "F(" + element.getFunction().getName() + ")";
		else if (element.isIdentifier())
			return "I(" + element.getIdentifier() + ")";
		else
			return "UnkonwnElement";
	}
	string to_string_2(const AST::Element &element) {
		if (element.isData())
			return element.getData().to_string_code();
		else if (element.isRefer())
			return "{" + std::to_string(element.getRefer()) + "}";
		else if (element.isDispData())
			return "Dp(" + element.getIdentifier() + ")";
		else if (element.isDispRefer())
			return "Dp{" + std::to_string(element.getRefer()) + "}";
		else if (element.isKeyword())
			return ICM::to_string(element.getKeyword());
		else if (element.isVariable())
			return element.getVariable().getName();
		else if (element.isFunction())
			return element.getFunction().getName();
		else if (element.isIdentifier())
			return element.getIdentifier();
		else
			return "UnkonwnElement";
	}
	string to_string(const vector<AST::Element> &vec) {
		return Convert::to_string(vec.begin(), vec.end(), [](const auto &e) { return ICM::to_string(e); });
	}
	string to_string(const AST::Node &node) {
		string str("[" + std::to_string(node.getIndex()) + "]: " + "(" + to_string((const vector<AST::Element> &)node) + ")");
		return str;
	}
	string to_string(const AST &ast) {
		string str("{AST:");
		if (ast.empty())
			str.append("NIL");
		else {
			for (const auto &e : ast.getTableRange()) {
				str.append("\n ");
				str.append(ICM::to_string(*e));
			}
		}
		str.append("\n}");
		return str;
	}
	// Old
	string to_string_old(const AST::Element &element) {
		if (element.isData())
			return "D(" + element.getData().to_string() + ")";
		else
			return "R[" + std::to_string(element.getRefer()) + "]";
	}
	string to_string_old(const AST::Node &node) {
		string str = "<N(" + std::to_string(node.getIndex()) + "):";
		for (auto &e : node)
			str.append(" " + to_string(e));
		str.append(">");
		return str;
	}
	string to_string_old(const AST &ast) {
		string str("{AST:");
		if (ast.empty())
			str.append("NIL");
		else {
			for (const auto &e : ast.getTableRange()) {
				str.append("\n ");
				str.append(ICM::to_string(*e));
			}
		}
		str.append("\n}");
		return str;
	}
}
