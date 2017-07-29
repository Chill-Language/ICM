#include "basic.h"
#include "parser/ast.h"
#include "temp-getelement.h"

namespace ICM
{
	//=======================================
	// * AST
	//=======================================
	string to_string(const AST::Element &element) {
		if (element.isLiteral())
			return "D(" + std::to_string(element.getIndex()) + ")";
			//return "D(" + getLiteral(element).to_string_code() + ")";
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
}
