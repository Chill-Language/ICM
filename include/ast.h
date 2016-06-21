#ifndef _ICM_AST_H_
#define _ICM_AST_H_

#include "basic.h"
#include "objects.h"

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
		FuncType getType() const {
			return this->type;
		}
		FuncID getID() const {
			return this->id;
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
		// Declare
		using List = std::vector<ASTNode*>;
		// Method
		void push(ASTNode *node) { list.push_back(node); }
		const List& getList() const {
			return list;
		}
		friend std::string to_string(const Parameters *pars);

		Parameters* clone() const; // Shallow Copy
		Parameters* deep_clone() const; // Deep Copy

	private:
		List list;
	};

	// ASTNode
	class ASTNode
	{
	public:
		ASTNode() {}
		// Create a Variables from Type.
		explicit ASTNode(ASTNodeType type) { initialize(type); }
		// Create a Reference from Pointer.
		explicit ASTNode(const ObjectPtr &dat) { initialize(dat); }
		explicit ASTNode(Function *fun, Parameters *par) { initialize(fun, par); }
		explicit ASTNode(Function *fun) : ASTNode(fun, nullptr) {}
		explicit ASTNode(Parameters *par) : ASTNode(nullptr, par) {}
		~ASTNode() { release(); }

		void initialize(ASTNodeType type);
		void initialize(const ObjectPtr &dat);
		void initialize(Function *fun = nullptr, Parameters *par = nullptr);

		void release();
		ASTNode* clone() const; // Shallow Copy
		ASTNode* deep_clone() const; // Deep Copy
		ASTNodeType getNodeType() const {
			return this->type;
		}
		// Get
		const ObjectPtr& getdata() const {
			return this->objdata;
		}
		const Function* getFunc() const {
			return this->fundata.func;
		}
		const Parameters* getPars() const {
			return this->fundata.pars;
		}
		// Set
		void setdata(const ObjectPtr &pdata) {
			this->objdata = pdata;
		}
		void setfunc(FuncType type, FuncID id) {
			this->fundata.func->set(type, id);
		}
		void pushpars(ASTNode *node) {
			this->fundata.pars->push(node);
		}
		DefaultType getObjtype() const {
			return objdata->get_type();
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
			ObjectPtr objdata = nullptr;
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
		const ASTNode* getRoot() const {
			return this->root;
		}
		bool isend() const {
			return farthptrs.empty();
		}
		AST* reset() {
			this->currptr = root;
			return this;
		}

		AST* setdata(const ObjectPtr &pdata) {
			this->currptr->setdata(pdata);
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

#endif
