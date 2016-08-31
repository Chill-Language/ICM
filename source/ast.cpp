#include "ast.h"

namespace ICM
{
	//=======================================
	// * Namespace ASTNode
	//=======================================
	namespace ASTNode
	{
		//=======================================
		// * Class ASTNodeNode
		//=======================================
		string ASTNodeNode::to_string() const {
			string str = "<N(" + std::to_string(index) + "):";
			for (auto p : data)
				str.append(" " + p->to_string());
			str.append(">");
			return str;
		}
		string ASTNodeNode::to_string_code() const {
			string str("[" + std::to_string(index) + "]: " + "(" + to_string_for_order() + ")");
			return str;
		}
		string ASTNodeNode::to_string_for_order() const {
			string str;
			//str.append("(");
			for (auto p : data)
				str.append(p->to_string_code() + " ");
			if (!data.empty())
				str.pop_back();
			//str.append(")");
			return str;
		}

		//=======================================
		// * Class ASTNodeData
		//=======================================
		string ASTNodeData::to_string() const {
			return "D(" + data->to_string() + ")";
		}
		string ASTNodeData::to_string_code() const {
			return data->to_string_code();
		}

		//=======================================
		// * Class ASTNodeRefer
		//=======================================
		string ASTNodeRefer::to_string() const {
			return "R[" + std::to_string(data) + "]";
		}
		string ASTNodeRefer::to_string_code() const {
			return "{" + std::to_string(data) + "}";
		}
	}

	//=======================================
	// * Class AST
	//=======================================
	void AST::pushData(const ObjectPtr &op) {
		currptr->pushNode(shared_ptr<Base>(new Data(op)));
	}
	void AST::pushNode() {
		currptr->pushNode(shared_ptr<Base>(new Refer(currindex)));
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
				str.append((*e)->to_string());
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
				str.append((*e)->to_string_code());
			}
		}
		str.append("\n}");
		return str;
	}
}
