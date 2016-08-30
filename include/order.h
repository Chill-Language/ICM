#ifndef _ICM_ORDER_H_
#define _ICM_ORDER_H_

#include "basic.h"
#include "ast.h"
#include "keyword.h"

namespace ICM
{
	namespace ASTOrder
	{
		class OrderData
		{
		public:
			enum Order { CALL, JUMP, JUMPIF, FUNC, CALLS, OVER, RET };

			virtual Order order() const = 0;

			string to_string() const {
				string str;
				switch (order())
				{
				case CALL:   str.append("CALL"); break;
				case CALLS:  str.append("CALS"); break;
				case JUMP:   str.append("JUMP"); break;
				case JUMPIF: str.append("JMPF"); break;
				case FUNC:   str.append("FUNC"); break;
				case RET:    str.append("RET "); break;
				case OVER:   str.append("OVER"); break;
				}
				str.append(" | ");
				str.append(getToString());
				return str;
			}

		private:
			virtual string getToString() const {
				return "";
			}
		};

		class OrderList;

		class OrderDataCall : public OrderData
		{
			using NodePtr = AST::Node*;
		public:
			OrderDataCall(NodePtr nodptr) : value(ObjectPtr()), nodptr(nodptr) {}
			OrderDataCall(ObjectPtr &objptr, NodePtr nodptr) : value(std::move(objptr)), nodptr(nodptr) {}
			OrderData::Order order() const { return OrderData::CALL; }
			ObjectPtr getValue() const { return value; }
			void setValue(const ObjectPtr &obj) { value = obj; }
			NodePtr& getData() { return nodptr; }
			const NodePtr& getData() const { return nodptr; }

		private:
			ObjectPtr &&value;
			NodePtr nodptr;
			string getToString() const {
				return nodptr->to_string_for_order();
			}
		};

		class OrderDataFunc : public OrderData
		{
		public:
			OrderData::Order order() const { return OrderData::FUNC; }
		private:
		};

		class OrderDataJump : public OrderData
		{
		public:
			OrderData::Order order() const { return OrderData::JUMP; }
		private:
		};

		class OrderDataJumpIf : public OrderData
		{
		public:
			OrderDataJumpIf(ObjectPtr op) : opdata(op) {}
			OrderData::Order order() const { return OrderData::JUMPIF; }
		private:
			ObjectPtr opdata;
			string getToString() const {
				return opdata->to_string_code();
			}
		};

		class OrderDataCalls : public OrderData
		{
		public:
			OrderData::Order order() const { return OrderData::CALLS; }
		private:
			shared_ptr<OrderList> data;
		};

		class OrderDataEnd : public OrderData
		{
		public:
			OrderData::Order order() const { return OrderData::OVER; }
		private:
		};

		class OrderDataRet : public OrderData
		{
		public:
			OrderData::Order order() const { return OrderData::RET; }
		private:
		};

		class OrderList
		{
		public:
			OrderList() {}
			void push() {

				curindex++;
			}

		private:
			vector<shared_ptr<OrderData>> data;
			size_t curindex = 0;
		};


		class CreateOrder
		{
		public:
			CreateOrder(vector<AST::NodePtr> &table) : table(table) {}
			vector<OrderDataCall*> createOrderASTs();

		private:
			vector<AST::NodePtr> &table;
			vector<OrderDataCall*> GlobalOrderDataList;
			map<size_t, size_t> GlobalOrderDataListReferMap;

			void createOrderASToneNode(AST::Node &node);
			void createOrderKeyword(AST::Node &node, KeywordID keyword);
			void createOrderIdentifier(AST::Node &node, Objects::Identifier *ident);
		};
	}
}

#endif
