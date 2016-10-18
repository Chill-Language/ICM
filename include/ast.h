#ifndef _ICM_AST_H_
#define _ICM_AST_H_

#include "basic.h"
#include "objects.h"

namespace ICM
{
	namespace ASTBase
	{
		//=======================================
		// * Class Element
		//=======================================
		class Element
		{
		public:
			enum EleType { E_Data, E_Refer };
		public:
			Element(EleType type) : type(type) {}
			//~Element() { if (isData()) delete data.op; }

			static Element Data(Objects::Object *op);
			static Element Refer(size_t index);

			EleType getType() const { return type; }

			bool isData() const { return type == E_Data; }
			bool isRefer() const { return type == E_Refer; }

			// Get/Set
			ObjectPtr& getData() const { return *data.op; }
			void setData(Objects::Object* op);

			size_t getRefer() const { return data.id; }
			void setRefer(size_t id) { data.id = id; }

		protected:
			EleType type;
			union {
				ObjectPtr* op;
				size_t id;
			} data;
		};

		//=======================================
		// * Class Node
		//=======================================
		class Node : public vector<Element>
		{
		public:
			explicit Node(size_t id) : index(id) {}

			// Index
			void setIndex(size_t id) { this->index = id; }
			size_t getIndex() const { return this->index; }

		private:
			size_t index;
		};
	}

	//=======================================
	// * Class AST
	//=======================================
	class AST
	{
	public:
		using Element = ASTBase::Element;
		using Node = ASTBase::Node;
		using NodePtr = shared_ptr<Node>;
	public:
		AST() : root(new Node(0)), currindex(1), table({ root }), currptr(root.get()) {}

		void pushData(Objects::Object *op);
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

	private:
		NodePtr root;
		size_t currindex = 0;
		vector<NodePtr> table;
		Node *currptr;
		stack<Node*> farthptrs;
	};

	std::string to_string(const AST *ast);
	string to_string(const AST &ast);
	string to_string_code(const AST &ast);
	string to_string(const AST::Element &element);
	string to_string_code(const AST::Element &element);
	string to_string(const AST::Node &node);
	string to_string_code(const AST::Node &node);
	string to_string_for_order(const AST::Node &node);
}

#endif
