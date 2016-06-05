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
	enum FuncType { FUNC_NIL, FUNC_DEF, FUNC_ADD };
	using FuncID = unsigned;
	enum ASTNodeType { AST_NIL, AST_DATA, AST_FUNC };

	// ObjectData
	class ObjectData
	{
	public:
		ObjectData() = default;
		ObjectData(const ObjectData &obj) {}
		template <typename T>
		explicit ObjectData(const T &data) {
			setData(data);
		}
		~ObjectData() { delete pointer; }

		template <typename T>
		void setData(const T & data) {
			if (!pointer)
				pointer = new T(data);
			else
				*getPointer<T>() = data;
		}
		template <typename T>
		T* getPointer() {
			return (T*)pointer;
		}
		template <typename T>
		const T* getPointer() const {
			return (const T*)pointer;
		}
		template <typename T>
		T getData() const {
			return *getPointer<T>();
		}
		template <typename T>
		void release() {
			getPointer<T>()->~T();
			pointer = nullptr;
		}

		friend std::string to_string(const ObjectData *obj);

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
		explicit ASTNode(ObjectData *dat) { initialize(dat); }
		explicit ASTNode(Function *fun, Parameters *par) { initialize(fun, par); }
		explicit ASTNode(Function *fun) : ASTNode(fun, nullptr) {}
		explicit ASTNode(Parameters *par) : ASTNode(nullptr, par) {}
		~ASTNode() { release(); }

		void initialize(ASTNodeType type) {
			this->reference = false;
			switch (type) {
			case AST_DATA:
				initialize(new ObjectData());
				break;
			case AST_FUNC:
				initialize(new Function(), new Parameters());
				break;
			default:
				this->type = type;
			}
		}
		void initialize(Function *fun = nullptr, Parameters *par = nullptr) {
			this->type = AST_FUNC;
			this->fundata.func = fun;
			this->fundata.pars = par;
		}
		void initialize(ObjectData *dat) {
			this->type = AST_DATA;
			this->objdata = dat;
		}
		void release() {
			if (reference)
				return;
			switch (type) {
			case AST_DATA:
				delete this->objdata;
				break;
			case AST_FUNC:
				delete this->fundata.func;
				delete this->fundata.pars;
				break;
			default:
				;
			}
		}

		template <typename T>
		void setdata(const T & data) {
			this->objdata->setData(data);
		}
		void setfunc(FuncType type, FuncID id) {
			this->fundata.func->set(type, id);
		}
		void pushpars(ASTNode *node) {
			this->fundata.pars->push(node);
		}

		friend std::string to_string(const ASTNode *astn);

	protected:
		// Members
		ASTNodeType type;
		union {
			struct {
				Function *func = nullptr;
				Parameters *pars = nullptr;
			} fundata;
			ObjectData *objdata = nullptr;
		};
		bool reference = true;
	};
}
