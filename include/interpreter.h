#ifndef _ICM_INTERPRETER_H_
#define _ICM_INTERPRETER_H_

#include "basic.h"
#include "order.h"
#include "objectdef.h"

namespace ICM
{
	class Interpreter
	{
		using OrderList = vector<ASTOrder::OrderData*>; // ASTOrder::OrderList;
	public:
		Interpreter(const OrderList &ol)
			: orderlist(ol), tempresult(ol.size(), ObjectPtr()) {}
		ObjectPtr run();
		DataList createList(const RangeIterator<vector<AST::Element>::iterator> &r);

	private:
		OrderList orderlist;
		ObjectPtr Result;
		vector<ObjectPtr> tempresult;

		struct {
			struct {
				Types::Function Func;
				DataList Args;
			} Func;
		} Global;

		void runFunc(const ObjectPtr &op, AST::Node *n, size_t id);
		void runSub(const ObjectPtr &op, AST::Node *node, size_t i);
		DataList getDataList(const vector<AST::Element*> &vb);
		ObjectPtr getObjectPtr(const AST::Element &e);
	};
}

#endif
