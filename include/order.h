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
			enum Order { CALL, JUMP, JUMPIFRR, JUMPIFVV, JUMPIFRV, JUMPIFVR, FUNC, CALLS, OVER, RET };

			virtual Order order() const = 0;

			string to_string() const {
				string str;
				switch (order())
				{
				case CALL:     str.append("CALL"); break;
				case CALLS:    str.append("CALS"); break;
				case JUMP:     str.append("JUMP"); break;
				case JUMPIFRR: str.append("JMPN"); break;
				case JUMPIFVV: str.append("JMPN"); break;
				case JUMPIFRV: str.append("JMPN"); break;
				case JUMPIFVR: str.append("JMPN"); break;
				case FUNC:     str.append("FUNC"); break;
				case RET:      str.append("RET "); break;
				case OVER:     str.append("OVER"); break;
				}
				str.append(" | ");
				str.append(getToString());
				return str;
			}
			virtual void adjustID(const map<size_t, size_t> &map) {}

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
			void adjustID(const map<size_t, size_t> &map) {
				for (auto &e : *nodptr) {
					if (e->getType() == AST::Refer::Type) {
						AST::Refer &r = static_cast<AST::Refer&>(*e);
						r.setData(map.at(r.getData()));
					}
				}
			}

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

		class OrderDataJumpNotIfRR : public OrderData
		{
		public:
			OrderDataJumpNotIfRR(size_t expid, size_t jmpid) : expid(expid), jmpid(jmpid) {}
			OrderData::Order order() const { return OrderData::JUMPIFRR; }
		private:
			size_t expid;
			size_t jmpid;
			string getToString() const {
				return "{" + std::to_string(expid) + "}, {" + std::to_string(jmpid) + "}";
			}
		};

		class OrderDataJumpNotIfVR : public OrderData
		{
		public:
			OrderDataJumpNotIfVR(ObjectPtr op, size_t jmpid) : opdata(op), jmpid(jmpid) {}
			OrderData::Order order() const { return OrderData::JUMPIFVR; }
		private:
			ObjectPtr opdata;
			size_t jmpid;
			string getToString() const {
				return opdata->to_string_code() + ", {" + std::to_string(jmpid) + "}";
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
			OrderDataRet(size_t id) : id(id) {}
			OrderData::Order order() const { return OrderData::RET; }
			void adjustID(const map<size_t, size_t> &map) {
				this->id = map.at(this->id);
			}
			size_t getID() const {
				return this->id;
			}

		private:
			size_t id;
			string getToString() const {
				return "{" + std::to_string(id) + "}";
			}
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
			CreateOrder(vector<AST::NodePtr> &table) : Table(table) {}
			vector<OrderData*>& createOrder();

		private:
			vector<AST::NodePtr> &Table;

			// New
			vector<OrderData*> OrderDataList;
			map<size_t, size_t> OrderDataListReferMap;



			// New
			using Single = AST::Node*;      // AST's Root
			using Segment = vector<Single>; // ASTs
			AST::Node* getReferNode(AST::Base *refer);
			ObjectPtr& getDataRef(AST::Base *data);
			void createOrderSub(const Single &single);
			void createOrderSub(const Segment &segment) {
				for (const Single &single : segment)
					createOrderSub(single);
			}
			void createOrderKeyword(const Single& single, KeywordID keyword);
			void setObjectIdentifier(ObjectPtr &op);
			void addOrder(const Single &single, OrderData *order) {
				size_t id = OrderDataList.size();
				if (OrderDataListReferMap.find(single->getIndex()) == OrderDataListReferMap.end()) {
					OrderDataListReferMap[single->getIndex()] = id;
					OrderDataList.push_back(order);
				}
				else {
					println("Occur a clash with index {", std::to_string(single->getIndex()), "->", std::to_string(id), "}.");
				}
			}
		};
	}
}

#endif
