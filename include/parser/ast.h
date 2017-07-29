#pragma once
#include "basic.h"
#include "keyword.h"
#include "objectenum.h"
#include "identifier.h"

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
				E_Void,
				E_Literal,
				E_Refer,
				E_Ident,
				E_Key,
			};

		public:
			Element() = default;
			Element(ElementType et, size_t id, size_t index)
				: etype((uint8_t)et), stype((uint8_t)id), index(index) {}
			Element(const Element&) = default;
			
			// Static Function
			static Element Literal(size_t type, size_t index) { return Element(E_Literal, type, index); }
			static Element Refer(size_t index) { return Element(E_Refer, 0, index); }
			static Element Keyword(Keyword::KeywordID key) { return Element(E_Key, key, 0); }
			static Element Identifier(size_t index) { return Element(E_Ident, 0, index); }
			static Element Identifier(IdentType type, size_t index) { return Element(E_Ident, type, index); }

			// Judge
			bool isLiteral() const { return isEltType(E_Literal); }
			bool isRefer() const { return isEltType(E_Refer); }
			bool isKeyword() const { return isEltType(E_Key); }
			bool isIdent() const { return isEltType(E_Ident); }

			bool isLiteralType(TypeUnit type) const { return isLiteral() && getSubType() == type; }
			bool isIdentType(IdentType type) const { return isIdent() && getSubType() == type; }

			// Get/Set Index
			void setIndex(size_t id) { index = id; }
			const size_t& getIndex() const { return index; }

			// Get/Set
			void setRefer(size_t id) { assert(isRefer()); setIndex(id); }
			size_t getRefer() const { assert(isRefer()); return getIndex(); }
			TypeUnit getLiteralType() const { assert(isLiteral()); return getSubType(); }
			size_t getIndetType() const { assert(isIdent()); return getSubType(); }
			Keyword::KeywordID getKeyword() const { assert(isKeyword()); return (Keyword::KeywordID)getSubType(); }

		private:
			union {
				struct { uint8_t etype, stype; };
				uint32_t type = 0;
			};
			uint_t index = 0;

		private:
			bool isEltType(ElementType et) const { return (ElementType)this->etype == et; }
			size_t getSubType() const { return (size_t)this->stype; }
		};

		//=======================================
		// * Class Node
		//=======================================
		class Node : public vector<Element>
		{
		public:
			explicit Node(size_t id) : index(id) {}
			Node(size_t id, vector<Element> &&VeE) : vector<Element>(VeE), index(id) {}

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
