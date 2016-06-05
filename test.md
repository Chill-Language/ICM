# Test Code
To Test The ASTNode, I give these codes:
```cpp
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

	// The Different of Ref & Cpy.
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

	// The Feature of DeepCpy.
	// sub->pushpars(root);  // Circle Ref, Error!
	println(root);
	cpy = root->clone(); // Shallow Copy, Error!
	//sub->pushpars(root);
	println(cpy);
	ASTNode *deep_cpy;
	deep_cpy = root->deep_clone(); // Deep Copy, Right!
	sub->pushpars(root);
	println(deep_cpy);
```
