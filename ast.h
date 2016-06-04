#include <vector>
#include "charptr.h"

namespace ICM
{
	// Declare
	class ObjectData;
	class Function;
	class Parameters;
	class ASTNode;
	// Types
	enum FuncType { FUNC_DEF, FUNC_ADD };
	using FuncID = unsigned;
	enum ASTNodeType { AST_DATA, AST_FUNC };

	// ObjectData
	class ObjectData
	{
	public:
		ObjectData() = default;
		template <typename T> explicit ObjectData(const T &data) { setData(data); }
		~ObjectData() { /*delete pointer;*/ }
		template <typename T> void setData(const T & data) { if (!pointer) pointer = new T(data); else *getPointer<T>() = data; }
		template <typename T> T* getPointer() { return (T*)pointer; }
		template <typename T> const T* getPointer() const { return (const T*)pointer; }
		template <typename T> T getData() const { return *getPointer<T>(); }
		template <typename T> void release() { getPointer<T>()->~T(); }
		std::string to_string() const {
			/*System::charptr data(sizeof(void*) * 2 + 2);
			sprintf(data, "0x%p", pointer);
			return std::string(data);*/
			return std::to_string(*(int*)pointer);
		}

	private:
		void *pointer = nullptr;
	};

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
		std::string to_string() const;

	private:
		// Members
		FuncType type;
		FuncID id = 0;
	};

	// Parameters
	class Parameters
	{
	public:
		Parameters() {}
		// Method
		void push(ASTNode *node) { list.push_back(node); }
		void push(ASTNode &node) { push(&node); }
		std::string to_string() const;

	private:
		std::vector<ASTNode*> list;
	};

	// ASTNode
	class ASTNode
	{
	public:
		ASTNode() {}
		explicit ASTNode(ObjectData &dat) :
			ASTNode(&dat) {}
		explicit ASTNode(Function &fun) :
			ASTNode(&fun) {}
		explicit ASTNode(Parameters &par) :
			ASTNode(&par) {}
		explicit ASTNode(Function &fun, Parameters &par) :
			type(AST_FUNC), func(&fun), pars(&par) {}

		explicit ASTNode(ObjectData *dat) :
			type(AST_DATA), data(dat) {}
		explicit ASTNode(Function *fun) :
			type(AST_FUNC), func(fun) {}
		explicit ASTNode(Parameters *par) :
			type(AST_FUNC), pars(par) {}
		explicit ASTNode(Function *fun, Parameters *par) :
			type(AST_FUNC), func(fun), pars(par) {}

		void initialize(ASTNodeType type);

		void setfunc(FuncType type, FuncID id) {
			this->func->set(type, id);
		}
		void pushpars(ASTNode *node) {
			this->pars->push(node);
		}
		void pushpars(ASTNode &node) {
			this->pars->push(node);
		}

		std::string to_string() const;

	protected:
		// Members
		ASTNodeType type;
		Function *func = nullptr;
		Parameters *pars = nullptr;
		ObjectData *data = nullptr;
	};

	// AST
	class AST
	{
	public:
		AST() {}

	private:
		ASTNode *pointer;
	};
}
