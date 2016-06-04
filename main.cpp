#include "prints.h"
#include "ast.h"
using std::string;
using std::vector;
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

	private:
		ASTNode *pointer;
	};
}

using namespace ICM;

int main(void)
{
	// (+ 5 6)
	AST ast;
	ASTNode *root;
	ObjectData *dat1, *dat2;
	ASTNode *adat1, *adat2;

	//ASTNode *root2 = new ASTNode();
	//root2->initialize(AST_FUNC);

	root = new ASTNode();
	root->initialize(AST_FUNC);

	root->setfunc(FUNC_DEF, 1);
	dat1 = new ObjectData(5);
	dat2 = new ObjectData(6);
	adat1 = new ASTNode(dat1);
	adat2 = new ASTNode(dat2);

	root->pushpars(adat1);
	root->pushpars(adat2);

	println(root);
	//println(*root2);
	

	return 0;
}
