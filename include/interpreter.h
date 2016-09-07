#ifndef _ICM_INTERPRETER_H_
#define _ICM_INTERPRETER_H_

#include "basic.h"
#include "order.h"

namespace ICM
{
	class Interpreter
	{
		using OrderList = vector<ASTOrder::OrderData*>; // ASTOrder::OrderList;
	public:
		Interpreter(const OrderList &ol)
			: orderlist(ol), tempresult(ol.size(), ObjectPtr()) {}
		ObjectPtr run();
		DataList createList(const Range<vector<shared_ptr<AST::Base>>::iterator> &r);

	private:
		OrderList orderlist;
		vector<ObjectPtr> tempresult;
		ObjectPtr Result;
		void runFunc(const ObjectPtr &op, AST::Node *n, size_t id);
		void runSub(const ObjectPtr &op, AST::Node *node, size_t i);
	};
}

#endif
