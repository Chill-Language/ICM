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
	AST ast;
	ASTNode *root;
	ASTNode *sub;
	ASTNode *dat1, *dat2, *dat3;

	// (+ 5 6)
	root = new ASTNode(AST_FUNC);

	root->setfunc(FUNC_DEF, 1);
	dat1 = new ASTNode(AST_DATA);
	dat2 = new ASTNode(AST_DATA);
	dat1->setdata(5);
	dat2->setdata(6);

	root->pushpars(dat1);
	root->pushpars(dat2);

	println(root);

	// (+ 7 (+ 5 6))
	sub = root;
	root = new ASTNode(AST_FUNC);
	root->setfunc(FUNC_DEF, 1);
	dat3 = new ASTNode(AST_DATA);
	dat3->setdata(7);
	root->pushpars(dat3);
	root->pushpars(sub);

	println(root);

	// (+ 5 6 7)
	sub->pushpars(dat3);

	println(sub);
	println(root);

	// The Differecnt of Ref & Cpy.
	// * Ref
	ASTNode *ref;
	ref = new ASTNode(AST_FUNC);
	ref->pushpars(dat3);
	println(ref);
	dat3->setdata(0);
	ref->pushpars(dat3);
	println(ref);
	// * Cpy
	ASTNode *cpy;
	cpy = new ASTNode(AST_FUNC);
	dat3->setdata(0);
	cpy->pushpars(dat3->clone());
	println(cpy);
	dat3->setdata(1);
	cpy->pushpars(dat3->clone());
	println(cpy);

	

	return 0;
}
