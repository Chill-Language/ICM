#include <vector>
#include <stack>
#include "type.h"
#include "objectdata.h"

namespace ICM
{
	// Declare
	class Function;
	class Parameters;
	class ASTNode;
	// Types
	enum FuncType { FUNC_NIL, FUNC_DEF, FUNC_ADD };
	using FuncID = unsigned;
	enum ASTNodeType { AST_NIL, AST_DATA, AST_FUNC };

	// Function
	class Function
	{
	public:
		Function() {}
		Function(FuncType type, FuncID id) : type(type), id(id) {}
		void set(FuncType type, FuncID id) {
			this->type = type;
			this->id = id;
		}
		friend std::string to_string(const Function* func);

	private:
		// Members
		FuncType type = FUNC_NIL;
		FuncID id = 0;
	};

	// Parameters
	class Parameters
	{
	public:
		Parameters() {}
		// Method
		void push(ASTNode *node) { list.push_back(node); }
		friend std::string to_string(const Parameters *pars);

		Parameters* clone() const; // Shallow Copy
		Parameters* deep_clone() const; // Deep Copy

	private:
		std::vector<ASTNode*> list;
	};

	// ASTNode
	class ASTNode
	{
	public:
		ASTNode() {}
		// Create a Variables from Type.
		explicit ASTNode(ASTNodeType type) { initialize(type); }
		// Create a Reference from Pointer.
		explicit ASTNode(ObjectData *dat, DefaultType type = T_Null) { initialize(dat, type); }
		explicit ASTNode(Function *fun, Parameters *par) { initialize(fun, par); }
		explicit ASTNode(Function *fun) : ASTNode(fun, nullptr) {}
		explicit ASTNode(Parameters *par) : ASTNode(nullptr, par) {}
		~ASTNode() { release(); }

		void initialize(ASTNodeType type);
		void initialize(ObjectData *dat, DefaultType type = T_Null);
		void initialize(Function *fun = nullptr, Parameters *par = nullptr);
		
		void release();
		ASTNode* clone() const; // Shallow Copy
		ASTNode* deep_clone() const; // Deep Copy

		template <typename T>
		void setdata(const T & data) {
			this->objdata.data->setData<T>(data);
		}
		template <typename T>
		T getdata() {
			return this->objdata.data->getData<T>();
		}
		void settype(DefaultType type) {
			this->objdata.type = type;
		}
		void setfunc(FuncType type, FuncID id) {
			this->fundata.func->set(type, id);
		}
		void pushpars(ASTNode *node) {
			this->fundata.pars->push(node);
		}

		friend std::string to_string(const ASTNode *astn);
		friend class AST;

	private:
		// Members
		ASTNodeType type;
		union {
			struct {
				Function *func = nullptr;
				Parameters *pars = nullptr;
			} fundata;
			struct {
				ObjectData *data = nullptr;
				DefaultType type = T_Null;
			} objdata;
		};
		bool reference = true;
	};

	// AST
	class AST
	{
	public:
		AST() {}
		AST* pushNode(ASTNodeType type);
		int retNode();
		bool isend() const {
			return farthptrs.empty();
		}
		
		template <typename T>
		AST* setdata(const T & data) {
			this->currptr->setdata<T>(data);
			return this;
		}
		AST* settype(DefaultType type) {
			this->currptr->settype(type);
			return this;
		}
		AST* setfunc(FuncType type, FuncID id) {
			this->currptr->setfunc(type, id);
			return this;
		}
		AST* pushpars(ASTNode *node) {
			this->currptr->pushpars(node);
			return this;
		}

		friend std::string to_string(const AST *ast);

	private:
		ASTNode *root = nullptr;
		ASTNode *currptr = nullptr;
		std::stack<ASTNode*> farthptrs;
	};
}