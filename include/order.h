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
			enum Order { CALL, SINGLE, STORE, JUMP, JUMPNOT, FUNC, OVER, RET };

			virtual Order order() const = 0;

			string to_string() const {
				string str;
				switch (order())
				{
				case CALL:     str.append("CALL"); break;
				case SINGLE:   str.append("SING"); break;
				case STORE:    str.append("STOR"); break;
				case JUMP:     str.append("JUMP"); break;
				case JUMPNOT:  str.append("JMPN"); break;
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
			OrderDataCall(NodePtr nodptr) : /*value(ObjectPtr()),*/ nodptr(nodptr) {}
			//OrderDataCall(ObjectPtr &objptr, NodePtr nodptr) : value(std::move(objptr)), nodptr(nodptr) {}
			OrderData::Order order() const { return OrderData::CALL; }
			//ObjectPtr getValue() const { return value; }
			//void setValue(const ObjectPtr &obj) { value = obj; }
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
			//ObjectPtr &&value;
			NodePtr nodptr;
			string getToString() const {
				return nodptr->to_string_for_order();
			}
		};

		class OrderDataSingle : public OrderData
		{
		public:
			OrderDataSingle(ObjectPtr objptr) : data(objptr) {}
			OrderData::Order order() const { return OrderData::SINGLE; }
			const ObjectPtr& getData() const { return data; }
			void setData(const ObjectPtr &obj) { data = obj; }

		private:
			ObjectPtr data;
			string getToString() const {
				return data->to_string_code();
			}
		};

		class OrderDataStore : public OrderData
		{
		public:
			OrderDataStore() {}
			OrderData::Order order() const { return OrderData::STORE; }
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
			virtual OrderData::Order order() const { return OrderData::JUMP; }
			void setJmpID(size_t jmpid) {
				this->jmpid = jmpid;
			}
			void setJmprefAdjusted() {
				this->adjusted = true;
			}
			virtual void adjustID(const map<size_t, size_t> &map) {
				if (!adjusted)
					this->jmpid = map.at(this->jmpid);
			}
			size_t getJmpid() const {
				return jmpid;
			}

		protected:
			size_t jmpid;
			bool adjusted = false;
			virtual string getToString() const {
				return "{" + std::to_string(jmpid) + "}";
			}
		};
		class OrderDataJumpNotIf : public OrderDataJump
		{
		public:
			OrderDataJumpNotIf(size_t expid)
				: expid(expid) {}
			OrderData::Order order() const { return OrderData::JUMPNOT; }
			void adjustID(const map<size_t, size_t> &map) {
				if (!exprefadjusted)
					this->expid = map.at(this->expid);
				OrderDataJump::adjustID(map);
			}
			size_t getExpid() const {
				return expid;
			}

			void setExprefAdjusted() {
				exprefadjusted = true;
			}

		protected:
			size_t expid;
			bool exprefadjusted = false;

			string getToString() const {
				return "{" + std::to_string(expid) + "}, " + OrderDataJump::getToString();
			}
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
				//this->id = map.at(this->id);
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
			using Segment = vector<AST::Base*>; // ASTs
			AST::Node* getReferNode(AST::Base *refer);
			ObjectPtr& getDataRef(AST::Base *data);
			void createOrderSub(const Single &single);
			void createOrderSub(const Segment &segment);
			void createOrderKeyword(const Single& single, KeywordID keyword);
			void setObjectIdentifier(ObjectPtr &op);
			void addRefer(const Single &single) {
				if (OrderDataListReferMap.find(single->getIndex()) == OrderDataListReferMap.end()) {
					size_t id = OrderDataList.size();
					OrderDataListReferMap[single->getIndex()] = id;
				}
			}
			void addOrder(const Single &single, OrderData *order) {
				size_t id = OrderDataList.size();
				if (OrderDataListReferMap.find(single->getIndex()) == OrderDataListReferMap.end()) {
					OrderDataListReferMap[single->getIndex()] = id;
					addOrder(order);
				}
				else {
					println("Occur a clash with index {", std::to_string(single->getIndex()), "->", std::to_string(id), "}.");
				}
			}
			void addOrder(OrderData *order) {
				OrderDataList.push_back(order);
			}
		};
	}
}

#endif
