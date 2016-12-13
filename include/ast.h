#pragma once
#include "basic.h"
#include "keyword.h"
#include "objectenum.h"

namespace ICM
{
	namespace ASTBase
	{
		//=======================================
		// * Struct Element
		//=======================================
		struct Element
		{
		private:
			enum ElementType {
				E_Data,
				E_Refer,
				E_Ident,
				E_Key,
			};
			enum IndetifierType {
				I_Variable,
				I_Function,
			};

		public:
			Element() = default;
			explicit Element(ElementType et, size_t id = 0) { setEltType(et); setSubType(id); }
			Element(const Element&) = default;

			static Element Data(size_t type, size_t index);
			static Element Refer(size_t index);
			static Element Keyword(Keyword::KeywordID key);
			static Element Identifier(const string &name);
			static Element Variable(size_t index);
			static Element Function(size_t index);

			bool isData() const { return getEltType() == E_Data; }
			bool isRefer() const { return getEltType() == E_Refer; }
			bool isKeyword() const { return getEltType() == E_Key; }
			bool isIdentifier() const { return getEltType() == E_Ident; }

			bool isVariable() const { return isIdentifier() && getSubType() == I_Variable; }
			bool isFunction() const { return isIdentifier() && getSubType() == I_Function; }

			bool isNumber() const { return isData() && getSubType() == T_Number; }
			bool isBoolean() const { return isData() && getSubType() == T_Boolean; }
			bool isString() const { return isData() && getSubType() == T_String; }

			// Get/Set
			TypeUnit getDataType() const { return getSubType(); }
			size_t getIndex() const { return data.index; }

			size_t getRefer() const;
			void setRefer(size_t id) { data.index = id; }

			Keyword::KeywordID getKeyword() const;
			bool getBoolean() const;

		private:
			struct {
				uint8_t etype = 0;
				uint8_t stype = 0;
			} prop;

			union {
				size_t index = 0;
				bool bvalue;
			} data;

			void setEltType(ElementType etype) { prop.etype = (uint8_t)etype; }
			ElementType getEltType() const { return (ElementType)prop.etype; }
			void setSubType(size_t id) { prop.stype = (uint8_t)id; }
			size_t getSubType() const { return (size_t)prop.stype; }

			Element& setIndex(size_t id) { data.index = id; return *this; }
		};

		//=======================================
		// * Class Node
		//=======================================
		class Node : public vector<Element>
		{
		public:
			explicit Node(size_t id) : index(id) {}
			Node(size_t id, vector<Element> &&VeE) : index(id), vector<Element>(VeE) {}

			void push_front(const Element &elt) {
				this->insert(this->begin(), elt);
			}
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
		using NodePtr = unique_ptr<Node>;
	public:
		AST() : currindex(1), root(new Node(0)), currptr(root) {
			Table.push_back(NodePtr(root));
		}

		void pushData(Element&& element);
		void pushNode();
		bool retNode();

		bool empty() const {
			return Table.size() == 1;
		}
		bool isend() const {
			return farthptrs.empty();
		}
		auto getTableRange() const {
			return rangei(Table.begin() + 1, Table.end());
		}
		vector<NodePtr>& getTable() {
			return Table;
		}

	private:
		vector<NodePtr> Table;
		size_t currindex = 0;
		Node* root;
		Node *currptr;
		stack<Node*> farthptrs;
	};

	string to_string(const AST &ast);
	string to_string(const AST::Element &element);
	string to_string(const AST::Node &node);
	string to_string(const vector<AST::Element> &vec);
}
