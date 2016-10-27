#ifndef _ICM_ORDER_H_
#define _ICM_ORDER_H_

#include "basic.h"
#include "ast.h"
#include "keyword.h"
#include "tabledata.h"
#include "objectdef.h"

namespace ICM
{
	namespace ASTOrder
	{
		class OrderData
		{
		public:
			enum Order {
				START,
				SINGLE, STORE,
				JUMP, JUMPNOT,
				FUNC,
				AT,
				PTI, // print ident
				LET, CPY, REF,
				//
				FARG, // farg => $(Global.Func.Args)
				FFUN, // ffun => $(Global.Func.Func)
				FCAL, // fcal $(Global.Func.Func), $(Global.Func.Args)
				//
				CALL, CCAL, CHKT,
				CPYS, REFS,
				EQU, SML, SME, LAR, LAE,
				INC,
				OVER,
			};

			OrderData(Order order) : _order(order) {}

			Order order() const { return _order; };

			string to_string() const {
				string str;
				switch (order())
				{
				case START:    str.append("BEG "); break;
				case CALL:     str.append("CALL"); break;
				case CHKT:     str.append("CHKT"); break;
				case CCAL:     str.append("CCAL"); break;
				case SINGLE:   str.append("SING"); break;
				case STORE:    str.append("STOR"); break;
				case JUMP:     str.append("JUMP"); break;
				case JUMPNOT:  str.append("JMPN"); break;
				case FUNC:     str.append("FUNC"); break;
				case AT:       str.append("AT  "); break;
				case PTI:      str.append("PTI "); break;
				case LET:      str.append("LET "); break;
				case CPY:      str.append("CPY "); break;
				case REF:      str.append("REF "); break;
				case CPYS:     str.append("CPYS"); break;
				case REFS:     str.append("REFS"); break;
				case FARG:     str.append("FARG"); break;
				case FFUN:     str.append("FFUN"); break;
				case FCAL:     str.append("FCAL"); break;
				case EQU:      str.append("EQU "); break;
				case SML:      str.append("SML "); break;
				case SME:      str.append("SME "); break;
				case LAR:      str.append("LAR "); break;
				case LAE:      str.append("LAE "); break;
				case INC:      str.append("INC "); break;
				case OVER:     str.append("END "); break;
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
			Order _order;
		};

		class OrderDataCheckCall : public OrderData
		{
			using NodePtr = AST::Node*;
		public:
			OrderDataCheckCall(const NodePtr &nodptr) : OrderData(CCAL), nodptr(nodptr) {}
			NodePtr& getData() { return nodptr; }
			const NodePtr& getData() const { return nodptr; }
			void adjustID(const map<size_t, size_t> &map) {
				for (auto &e : *nodptr) {
					if (e.isRefer()) {
						AST::Element &r = static_cast<AST::Element&>(e);
						r.setRefer(map.at(r.getRefer()));
					}
				}
			}

		private:
			NodePtr nodptr;
			string getToString() const {
				return to_string_for_order(*nodptr);
			}
		};

		class OrderDataCall : public OrderData
		{
		public:
			OrderDataCall(const FuncTableUnit &ftu, size_t id, const vector<AST::Element*> &args)
				: OrderData(CALL), ftu(ftu), id(id), args(args) {}
			vector<AST::Element*>& getData() { return args; }
			const vector<AST::Element*>& getData() const { return args; }
			ObjectPtr call(const DataList &dl) {
				return ftu[id].call(dl);
			}
			void adjustID(const map<size_t, size_t> &map) {
				for (auto &e : args) {
					if (e->isRefer()) {
						AST::Element &r = static_cast<AST::Element&>(*e);
						r.setRefer(map.at(r.getRefer()));
					}
				}
			}

		private:
			const FuncTableUnit &ftu;
			size_t id;
			vector<AST::Element*> args;
			string getToString() const {
				string str(ftu.getName());
				for (auto &e : args)
					str.append(to_string_code(*e) + ", ");
				return str;
			}
		};
		//////////////////////////////////////
		struct OrderDataFuncArgs : public OrderData
		{
			using NodeList = vector<AST::Element>;
		public:
			OrderDataFuncArgs(const NodeList &args)
				: OrderData(FARG), Args(args) {}
			NodeList Args;
			void adjustID(const map<size_t, size_t> &map) {
				for (auto &e : Args) {
					if (e.isRefer()) {
						AST::Element &r = static_cast<AST::Element&>(e);
						r.setRefer(map.at(r.getRefer()));
					}
				}
			}

		private:
			string getToString() const {
				string str;
				for (auto &e : Args)
					str.append(to_string_code(e) + ", ");
				if (Args.size()) {
					str.pop_back();
					str.pop_back();
				}
				return str;
			}
		};
		struct OrderDataFuncFunc : public OrderData
		{
			OrderDataFuncFunc(const AST::Element &e)
				: OrderData(FFUN), Data(e) {}
			const AST::Element& Data;

		private:
			string getToString() const {
				return to_string_code(Data);
			}
		};
		//////////////////////////////////////

		class OrderDataCheckType : public OrderData
		{
		public:
			OrderDataCheckType(AST::Element* bp, DefaultType type) : OrderData(CHKT), bp(bp) {}
			AST::Element* getData() const { return bp; }
			DefaultType getType() const { return type; }

		private:
			AST::Element *bp;
			DefaultType type;
			string getToString() const {
				return to_string_code(*bp) + ", " + ICM::to_string(type);
			}
		};

		class OrderDataSingle : public OrderData
		{
		public:
			OrderDataSingle(ObjectPtr objptr) : OrderData(SINGLE), data(objptr) {}
			const ObjectPtr& getData() const { return data; }
			void setData(const ObjectPtr &obj) { data = obj; }

		private:
			ObjectPtr data;
			string getToString() const {
				return data->to_string_code();
			}
		};

		class OrderDataFunc : public OrderData
		{
		public:
			OrderDataFunc() : OrderData(FUNC) {}
		private:
		};

		class OrderDataJump : public OrderData
		{
		public:
			OrderDataJump() : OrderData(JUMP) {}
			OrderDataJump(Order order) : OrderData(order) {}
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
				: OrderDataJump(JUMPNOT), expid(expid) {}
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

		class OrderDataAt : public OrderData
		{
		public:
			OrderDataAt(size_t ref, size_t id) : OrderData(AT), ref(ref), id(id) {}
			size_t getRefid() const { return ref; }
			size_t getIndex() const { return id; }

		private:
			size_t ref;
			size_t id;
			string getToString() const {
				return "{" + std::to_string(ref) + "}, " + std::to_string(id);
			}
		};

		class OrderDataPrintIdent : public OrderData
		{
		public:
			OrderDataPrintIdent(const vector<size_t> &dat) : OrderData(PTI), data(dat) {}
			const vector<size_t>& getData() const { return data; }

		private:
			vector<size_t> data;
			string getToString() const {
				string str;
				for (auto &i : data) {
					str.push_back('{');
					str.append(std::to_string(i));
					str.append("}, ");
				}
				if (!data.empty()) {
					str.pop_back();
					str.pop_back();
				}
				return str;
			}
		};

		class OrderDataAssign : public OrderData
		{
		public:
#if USE_VARIABLE
			OrderDataAssign(Order order, VariableTableUnit &vtu, size_t id) : OrderData(order), id(id) {
				this->data = &vtu;
			}
			OrderDataAssign(Order order, ObjectPtr objptr, size_t id) : OrderData(order), id(id) {
				if (objptr.isType(T_Identifier)) {
					auto &n = objptr->get<T_Identifier>()->getName();
					this->data = &AddVariableTable[n];
				}
				else {
					println("Error in OrderDataAssign.");
				}
			}
			ObjectPtr getData() const { return ObjectPtr(data->getData()); }
			void setData(const ObjectPtr &obj) { data->setData(obj.get()); }
			size_t getRefid() const { return id; }

		private:
			VariableTableUnit *data;
			size_t id;
			string getToString() const {
				return data->getData()->to_string_code() + ", {" + std::to_string(id) + "}";
			}
#else
			OrderDataAssign(Order order, ObjectPtr objptr, size_t id) : OrderData(order), data(objptr), id(id) {}
			const ObjectPtr& getData() const { return data; }
			void setData(const ObjectPtr &obj) { data = obj; }
			size_t getRefid() const { return id; }

		private:
			ObjectPtr data;
			size_t id;
			string getToString() const {
				return data->to_string_code() + ", {" + std::to_string(id) + "}";
			}
#endif
		};

		class OrderDataCpySingle : public OrderData
		{
		public:
			OrderDataCpySingle(Order order, size_t id) : OrderData(order), id(id) {}
			size_t getRefid() const { return id; }

		private:
			size_t id;
			string getToString() const {
				return "{" + std::to_string(id) + "}";
			}
		};

		class OrderDataCompare : public OrderData
		{
		public:
			OrderDataCompare(Order order, ObjectPtr objptr, size_t id) : OrderData(order), data(objptr), id(id) {}
			const ObjectPtr& getData() const { return data; }
			void setData(const ObjectPtr &obj) { data = obj; }
			size_t getRefid() const { return id; }

		private:
			ObjectPtr data;
			size_t id;
			string getToString() const {
				return data->to_string_code() + ", {" + std::to_string(id) + "}";
			}
		};

		class OrderDataInc : public OrderData
		{
		public:
			OrderDataInc(ObjectPtr objptr) : OrderData(INC), data(objptr) {}
			const ObjectPtr& getData() const { return data; }
			void setData(const ObjectPtr &obj) { data = obj; }

		private:
			ObjectPtr data;
			string getToString() const {
				return data->to_string_code();
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
			stack<size_t> KeywordNodeIDs;



			// New
			using Single = AST::Node*;      // AST's Root
			using Segment = vector<AST::Element*>; // ASTs
			AST::Node* getReferNode(AST::Element &refer);
			ObjectPtr& getDataRef(AST::Element &data);
			void createSingle(AST::Element &bp);
			void createOrderSub(const Single &single);
			void createOrderSub(const Segment &segment);
			void createOrderKeyword(const Single& single, KeywordID keyword);
			void createOrderKeywordSingle(KeywordID keyword);
			VariableTableUnit& setObjectIdentifier(ObjectPtr &op);
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
