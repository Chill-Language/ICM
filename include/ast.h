#ifndef _ICM_AST_H_
#define _ICM_AST_H_

#include "basic.h"
#include "object.h"
#include "keyword.h"
#include "tabledata.h"
#include "typebase.h"

namespace ICM
{
	class ElementMemoryPool
	{
	public:
		template <typename T>
		size_t insert(const T &t) {
			size_t size = data.size();
			data.resize(size + sizeof(T));
			dat<T>(size) = t;
			return size;
		}
		size_t insert(const void *ptr, size_t len) {
			size_t size = data.size();
			data.resize(size + len);
			memcpy(get(size), ptr, len);
			return size;
		}
		template <typename T = void>
		T* get(size_t index) {
			return reinterpret_cast<T*>(data.data() + index);
		}
		template <typename T>
		T& dat(size_t index) {
			return *get<T>(index);
		}
		size_t index() const {
			return data.size();
		}
		void clear() {
			data.clear();
		}

	private:
		vector<byte> data;
	};

	extern ElementMemoryPool GlobalElementObjectPool;

	namespace ASTBase
	{
		//=======================================
		// * Struct Element
		//=======================================
		struct Element
		{
		public:
			enum EleType {
				E_Refer,
				
				E_Keyword,
				E_Identifier,

				E_Nil, // TODO
				E_Int,
				E_Number,
				E_String,
				E_Boolean,

				E_Variable,
				E_Function,
				E_DispData,
				E_DispRefer,
			};
		public:
			Element() = default;
			explicit Element(EleType type) : type(type) {}
			Element(const Element&) = default;

			static Element Data(size_t type, size_t index);
			static Element Refer(size_t index);
			static Element Keyword(Keyword::KeywordID key);
			static Element Identifier(const string &name);
			static Element Variable(size_t index);
			static Element Function(size_t index);

			bool isData() const { return isNumber() || isBoolean() || isString() || type == E_Nil; }
			bool isRefer() const { return type == E_Refer; }
			bool isKeyword() const { return type == E_Keyword; }
			bool isIdentifier() const { return type == E_Identifier; }
			bool isVariable() const { return type == E_Variable; }
			bool isFunction() const { return type == E_Function; }
			bool isDisp() const { return isDispData() || isDispRefer(); }
			bool isDispData() const { return type == E_DispData; }
			bool isDispRefer() const { return type == E_DispRefer; }

			bool isNumber() const { return type == E_Number; }
			bool isBoolean() const { return type == E_Boolean; }
			bool isString() const { return type == E_String; }

			// Get/Set
			Object getData() const;

			size_t getRefer() const {
				assert(isRefer() || isDispRefer());
				return data.index;
			}
			void setRefer(size_t id) { data.index = id; }
			
			Keyword::KeywordID getKeyword() const {
				assert(isKeyword());
				return data.key;
			}
			const string& getIdentifier() const;
			VariableTableUnit& getVariable() const;
			FuncTableUnit& getFunction() const;
			bool getBoolean() const {
				return data.bvalue;
			}

			Element& setDisp() {
				assert(isRefer() || isIdentifier());
				if (type == E_Refer)
					type = E_DispRefer;
				else
					type = E_DispData;
				return *this;
			}

		private:
			EleType type;
			union {
				size_t index;
				Keyword::KeywordID key;
				int ivalue;
				bool bvalue;
			} data;
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

#endif
