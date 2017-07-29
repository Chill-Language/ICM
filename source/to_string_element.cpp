#include "basic.h"
#include "parser/ast.h"
#include "tabledata.h"
#include "temp-getelement.h"

namespace ICM
{
	//=======================================
	// * AST
	//=======================================
	string to_string(const AST::Element &element) {
		if (element.isLiteral())
			return "D(" + getLiteral(element).to_string_code() + ")";
		else if (element.isRefer())
			return "R{" + std::to_string(element.getRefer()) + "}";
		else if (element.isKeyword())
			return "K(" + ICM::to_string(element.getKeyword()) + ")";
		else if (element.isIdentType(I_DyVarb))
			return "V(" + getIdentName(element) + ")";
		else if (element.isIdentType(I_StFunc))
			return "F(" + getIdentName(element) + ")";
		else if (element.isIdent())
			return "I(" + getIdentName(element) + ")";
		else
			return "UnkonwnElement";
	}
	string to_string_2(const AST::Element &element) {
		if (element.isLiteral())
			return getLiteral(element).to_string_code();
		else if (element.isRefer())
			return "{" + std::to_string(element.getRefer()) + "}";
		else if (element.isKeyword())
			return ICM::to_string(element.getKeyword());
		else if (element.isIdentType(I_DyVarb))
			return getIdentName(element);
		else if (element.isIdentType(I_StFunc))
			return getIdentName(element);
		else if (element.isIdent())
			return getIdentName(element);
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
		if (element.isLiteral())
			return "D(" + getLiteral(element).to_string() + ")";
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
