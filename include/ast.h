#ifndef _ICM_AST_H_
#define _ICM_AST_H_

#include "basic.h"
#include "objects.h"

namespace ICM
{
	//=======================================
	// * Namespace ASTNode
	//=======================================
	namespace ASTNode
	{
		enum ASTNodeType { AST_Node, AST_Data, AST_Refer };

		//=======================================
		// * Class ASTNodeBase
		//=======================================
		class ASTNodeBase
		{
		public:
			virtual ASTNodeType getType() const = 0;
			virtual string to_string() const = 0;
			virtual string to_string_code() const = 0;
		};

		//=======================================
		// * Class ASTNodeNode
		//=======================================
		class ASTNodeNode : public ASTNodeBase
		{
			using ASTNodePtr = shared_ptr<ASTNodeBase>;
		public:
			explicit ASTNodeNode(size_t id) : index(id) {}
			// List
			ASTNodeBase* front() const { return data.front().get(); }
			bool empty() const { return data.empty(); }
			void pushNode(const ASTNodePtr &node) { data.push_back(node); }
			auto begin() const { return data.begin(); }
			auto end() const { return data.end(); }
			auto begin() { return data.begin(); }
			auto end() { return data.end(); }
			auto cbegin() const { return data.cbegin(); }
			auto cend() const { return data.cend(); }
			size_t size() const { return data.size(); }
			const ASTNodePtr& operator[](size_t id) const { return data[id]; }

			// Index
			void setIndex(size_t id) { this->index = id; }
			size_t getIndex() const { return this->index; }
			string to_string_for_order() const;

			//-----------------------------------
			// + Inherited
			//-----------------------------------
			ASTNodeType getType() const { return Type; }
			string to_string() const;
			string to_string_code() const;
			// Const
			static const ASTNodeType Type = AST_Node;

		private:
			size_t index;
			vector<ASTNodePtr> data;
		};

		//=======================================
		// * Class ASTNodeData
		//=======================================
		class ASTNodeData : public ASTNodeBase
		{
		public:
			ASTNodeData(const ObjectPtr &op) : data(op) {}
			const ObjectPtr& getData() const { return data; }
			ObjectPtr& getData() { return data; }
			//-----------------------------------
			// + Inherited
			//-----------------------------------
			ASTNodeType getType() const { return Type; }
			string to_string() const;
			string to_string_code() const;
			// Const
			static const ASTNodeType Type = AST_Data;

		private:
			ObjectPtr data;
		};

		//=======================================
		// * Class ASTNodeRefer
		//=======================================
		class ASTNodeRefer : public ASTNodeBase
		{
		public:
			ASTNodeRefer(size_t index) : data(index) {}
			size_t getData() const { return data; }
			void setData(size_t id) { data = id; }
			//-----------------------------------
			// + Inherited
			//-----------------------------------
			ASTNodeType getType() const { return Type; }
			string to_string() const;
			string to_string_code() const;
			// Const
			static const ASTNodeType Type = AST_Refer;

		private:
			size_t data;
		};
	}

	//=======================================
	// * Class AST
	//=======================================
	class AST
	{
	public:
		using Base = ASTNode::ASTNodeBase;
		using Node = ASTNode::ASTNodeNode;
		using Data = ASTNode::ASTNodeData;
		using Refer = ASTNode::ASTNodeRefer;
		using NodePtr = shared_ptr<Node>;

		AST() : root(new Node(0)), currindex(1), table({ root }), currptr(root.get()) {}

		void pushData(const ObjectPtr &op);
		void pushNode();
		bool retNode();
		AST* reset() {
			this->currptr = root.get();
			return this;
		}
		bool empty() const {
			return table.size() == 1;
		}
		bool isend() const {
			return farthptrs.empty();
		}
		auto getTableRange() const {
			return range(table.begin() + 1, table.end());
		}
		const vector<NodePtr>& getTable() const {
			return table;
		}
		Node* getRoot() const {
			return root.get();
		}
		string to_string() const;
		string to_string_code() const;

	private:
		NodePtr root;
		size_t currindex = 0;
		vector<NodePtr> table;
		Node *currptr;
		std::stack<Node*> farthptrs;
	};
}

#endif
