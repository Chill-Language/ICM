#include "prints.h"
#include "ast.h"
#include <stack>
using std::string;
using std::vector;
using std::stack;
using namespace System;

template <typename T>
void println(const T *obj)
{
	Output::println(to_string(obj));
}
template <typename T>
void println(const T &obj)
{
	Output::println(to_string(obj));
}

namespace ICM
{
	// AST
	class AST
	{
	public:
		AST() {}
		void pushNode(ASTNodeType type) {
			if (root == nullptr) {
				root = new ASTNode(type);
				currptr = root;
				farthptrs.push(currptr);
			}
			else {
				ASTNode *tmp = new ASTNode(type);
				if (currptr->type == AST_FUNC)
					currptr->pushpars(tmp);
				else
					farthptrs.top()->pushpars(tmp);
				currptr = tmp;
				if (tmp->type == AST_FUNC)
					farthptrs.push(currptr);
			}
		}
		template <typename T>
		void setdata(const T & data) {
			this->currptr->setdata<T>(data);
		}
		void setfunc(FuncType type, FuncID id) {
			this->currptr->setfunc(type, id);
		}
		void pushpars(ASTNode *node) {
			this->currptr->pushpars(node);
		}
		void retNode() {
			if (currptr != root) {
				currptr = farthptrs.top();
				farthptrs.pop();
			}
			else {
				System::Output::println("Error in retNode().");
			}
		}
		friend std::string to_string(const AST *ast);

	private:
		ASTNode *root = nullptr;
		ASTNode *currptr = nullptr;
		stack<ASTNode*> farthptrs;
	};

	std::string to_string(const AST *ast)
	{
		return std::string("{AST | ") + to_string(ast->root) + std::string("}");
	}
}

using namespace ICM;

int main(void)
{
	AST *ast = new AST();
	ASTNode *dat;
	// (+ 5 6)
	ast->pushNode(AST_FUNC);
	ast->pushNode(AST_DATA);
	ast->setdata(5);
	ast->pushNode(AST_DATA);
	ast->setdata(6);
	//ast->retNode();

	println(ast);

	return 0;
}
