#include "ast.h"

namespace ICM
{
	//=======================================
	// * Struct Element
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
	//=======================================
	// * Class Node
	//=======================================
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

	//=======================================
	// * Class AST
	//=======================================
	void AST::pushData(const ObjectPtr &op) {
		currptr->push_back(Element::Data(op));
	}
	void AST::pushNode() {
		currptr->push_back(Element::Refer(currindex));
		NodePtr tmp(new Node(currindex++));
		farthptrs.push(currptr);
		currptr = tmp.get();
		table.push_back(tmp);
	}
	bool AST::retNode() {
		if (!farthptrs.empty()) {
			currptr = farthptrs.top();
			farthptrs.pop();
			return true;
		}
		return false;
	}
	string AST::to_string() const {
		if (root == nullptr)
			return "Null";
		string str("{AST:");
		if (root->empty())
			str.append("NIL");
		else {
			for (const auto &e : getTableRange()) {
				str.append("\n ");
				str.append(ICM::to_string(**e));
			}
		}
		str.append("\n}");
		return str;
	}
	string AST::to_string_code() const {
		string str("{AST:");
		if (root->empty())
			str.append("NIL");
		else {
			for (const auto &e : getTableRange()) {
				str.append("\n ");
				str.append(ICM::to_string_code(**e));
			}
		}
		str.append("\n}");
		return str;
	}
}
