#include "order.h"
#include "objectsdef.h"
#include "config.h"

namespace ICM
{
	namespace ASTOrder
	{
		//=======================================
		// * Functions
		//=======================================
		ObjectPtr& getNodeDataRef(AST::Base *node)
		{
			return static_cast<AST::Data*>(node)->getData();
		}
		const ObjectPtr& getNodeDataValue(const AST::Base *node)
		{
			return static_cast<const AST::Data*>(node)->getData();
		}
		size_t getNodeRefer(const AST::Base *node)
		{
			return static_cast<const AST::Refer*>(node)->getData();
		}
		bool key_is(const AST::Base *node, KeywordID key)
		{
			using Key = Objects::Keyword;
			if (node->getType() != AST::Data::Type)
				return false;
			ObjectPtr op = getNodeDataValue(node);
			return op.isType<Key>() && op.get<Key>()->getData() == key;
		}
		void error(const string &msg = "")
		{
			println("Error ", msg);
		}

		//=======================================
		// * Order If
		//=======================================
		class IfStruct
		{
			using BasePtr = AST::Base*;
			using DoExps = vector<BasePtr>;
			struct Data
			{
				Data(BasePtr judexp, const DoExps &doexps)
					: judexp(judexp), doexps(doexps) {}
				BasePtr judexp;
				DoExps doexps;
			};
		public:
			IfStruct() {}

			void add(BasePtr judexp, const DoExps &doexps) {
				ifexps.push_back(Data(judexp, doexps));
			}
			void add_elexp(BasePtr e) {
				elexp.push_back(e);
			}
			const vector<Data>& getIfexps() const {
				return ifexps;
			}
			vector<Data>& getIfexps() {
				return ifexps;
			}
			const DoExps& getElfexp() const {
				return elexp;
			}
			DoExps& getElfexp() {
				return elexp;
			}

		private:
			vector<Data> ifexps;
			DoExps elexp;
		};

		void parseToOrderIf(const AST::Node &node, const Range<size_t> &range, IfStruct &ifstruct);

		IfStruct parseToOrderIf(const AST::Node &node)
		{
			IfStruct ifstruct;
			parseToOrderIf(node, Range<size_t>(1, node.size()), ifstruct);
			return ifstruct;
		}

		void parseToOrderIf(const AST::Node &node, const Range<size_t> &range, IfStruct &ifstruct)
		{
			if (range.size() < 2) {
				error();
				return;
			}
			AST::Base* judexp = node[range.begin()].get();
			vector<AST::Base*> doexps;
			enum { IF, ELSE, ELSEIF } mode = IF;
			bool startelse = false;
			size_t begin = (key_is(node[2].get(), KeywordID::THEN)) ? *range.begin() + 2 : *range.begin() + 1;
			for (size_t i : Range<size_t>(begin, range.end())) {
				AST::Base *e = node[i].get();
				switch (mode)
				{
				case IF:
					if (e->getType() == ASTNode::AST_Refer) {
						doexps.push_back(e);
					}
					else  if (key_is(e, KeywordID::ELSE)) {
						mode = ELSE;
						startelse = true;
					}
					else if (key_is(e, KeywordID::ELSIF)) {
						mode = ELSEIF;
					}
					else {
						doexps.push_back(e);
					}
					break;
				case ELSE:
					if (e->getType() == ASTNode::AST_Refer) {
						ifstruct.add_elexp(e);
					}
					else if (key_is(e, KeywordID::IF)) {
						mode = ELSEIF;
					}
					else {
						ifstruct.add_elexp(e);
					}
					startelse = false;
					break;
				case ELSEIF:
					ifstruct.add(judexp, doexps);
					parseToOrderIf(node, Range<size_t>(i, node.size()), ifstruct);
					return;
				}
			}
			if (startelse)
				error();
			ifstruct.add(judexp, doexps);
		}

		//=======================================
		// * Order Loop
		//=======================================
		class LoopStruct
		{
			using BasePtr = AST::Base*;
			using DoExps = vector<BasePtr>;
		public:
			LoopStruct() {}
			void setDoexps(const DoExps &doexps) {
				this->doexps = doexps;
			}
			const DoExps& getDoexps() const {
				return doexps;
			}

		private:
			DoExps doexps;
		};
		void parseToOrderLoop(const AST::Node &node, LoopStruct &loopstruct);
		LoopStruct parseToOrderLoop(const AST::Node &node) {
			LoopStruct loopstruct;
			parseToOrderLoop(node, loopstruct);
			return loopstruct;
		}
		void parseToOrderLoop(const AST::Node &node, LoopStruct &loopstruct) {
			vector<AST::Base*> doexps;
			for (auto e : range(node.begin()+1,node.end())) {
				doexps.push_back(e->get());
			}
			loopstruct.setDoexps(doexps);
		}

		//=======================================
		// * Create Order
		//=======================================
		AST::Node* CreateOrder::getReferNode(AST::Base *refer) {
			return Table[static_cast<AST::Refer*>(refer)->getData()].get();
		}
		ObjectPtr& CreateOrder::getDataRef(AST::Base *data) {
			return static_cast<AST::Data*>(data)->getData();
		}

		// New
		vector<OrderData*>& CreateOrder::createOrder() {
			// Create Order
			createOrderSub(getReferNode(Table[0]->front()));
			addOrder(Table[0].get(), new OrderDataRet(OrderDataList.size() - 1/*getReferNode(Table[0]->front())->getIndex()*/));
			// Adjust Refer
			//if (false)
			for (auto &e : OrderDataList)
				e->adjustID(OrderDataListReferMap);
			// Print Order
			if (GlobalConfig.PrintOrder) {
				println();
				for (size_t i : range(0, OrderDataList.size()))
					println("[", std::to_string(i), "] ", OrderDataList[i]->to_string());
				println();

				for (size_t i : range(0, OrderDataListReferMap.size())) {
					if (OrderDataListReferMap.find(i) != OrderDataListReferMap.end())
						println(std::to_string(i), " -> ", std::to_string(OrderDataListReferMap[i]));
				}
			}
			return OrderDataList;
		}
		void CreateOrder::createOrderSub(const Single& single) {
			if (GlobalConfig.PrintOrder)
				println(single->to_string_code());
			AST::Base *front = single->front();
			if (front->getType() == AST::Data::Type) {
				ObjectPtr &op = getDataRef(front);
				if (op.isType<Objects::Keyword>()) {
					createOrderKeyword(single, op.get<Objects::Keyword>()->getData());
					return;
				}
			}
			for (auto p : *single) {
				if (p->getType() == AST::Data::Type) {
					ObjectPtr &op = getDataRef(p.get());
					if (op.isType<Objects::Identifier>())
						setObjectIdentifier(op);
				}
				else if (p->getType() == AST::Refer::Type) {
					createOrderSub(getReferNode(p.get()));
				}
			}
			addOrder(single, new ASTOrder::OrderDataCall(single));
		}
		void CreateOrder::createOrderSub(const Segment &segment) {
			for (AST::Base* single : segment) {
				
				if (single->getType() == AST::Refer::Type)
					createOrderSub(getReferNode(single));
				else if (single->getType() == AST::Data::Type) {
					ObjectPtr &op = getDataRef(single);
					if (op.isType(T_Keyword)) {
						createOrderKeywordSingle(op.get<Objects::Keyword>()->getData());
						continue;
					}
					if (op.isType(T_Identifier))
						setObjectIdentifier(op);
					addOrder(new OrderDataSingle(op));
				}
			}
		}
		void CreateOrder::createOrderKeywordSingle(KeywordID keyword) {
			switch (keyword) {
			case KeywordID::BREAK: {
				if (KeywordNodeIDs.empty()) {
					error("Unmatch 'break'.");
				}
				OrderDataJump *p = new OrderDataJump();
				size_t id = KeywordNodeIDs.top();
				p->setJmpID(id);
				addOrder(p);
				break;
			}
			}
		}
		void CreateOrder::createOrderKeyword(const Single& single, KeywordID keyword) {
			switch (keyword) {
			case KeywordID::IF: {
				const IfStruct &ifstruct = parseToOrderIf(*single);
				auto &ifexps = ifstruct.getIfexps();
				auto &elfexp = ifstruct.getElfexp();

				vector<OrderDataJump*> recJmpToEnd;
				for (size_t i : range(0, ifexps.size())) {
					auto &judexp = ifexps[i].judexp;
					auto &doexps = ifexps[i].doexps;
					OrderDataJumpNotIf *pjmprr;
					if (judexp->getType() == AST::Refer::Type) {
						AST::Node *node = getReferNode(judexp);
						createOrderSub(node);
						pjmprr = new OrderDataJumpNotIf(node->getIndex());
					}
					else { // AST::Data
						ObjectPtr &op = getDataRef(judexp);
						if (op.isType(T_Identifier))
							setObjectIdentifier(op);
						addOrder(new OrderDataSingle(op));
						size_t id = OrderDataList.size() - 1;
						pjmprr = new OrderDataJumpNotIf(id);
						pjmprr->setExprefAdjusted();
					}
					addOrder(pjmprr);
					createOrderSub(doexps);
					OrderDataJump *p = new OrderDataJump();
					recJmpToEnd.push_back(p);
					addOrder(p);
					pjmprr->setJmpID(OrderDataList.size());
					pjmprr->setJmprefAdjusted();
					// Create Else
					if (i == ifexps.size() - 1) {
						if (elfexp.empty())
							addOrder(new OrderDataSingle(ObjectPtr(new Objects::Nil())));
						else
							createOrderSub(elfexp);
					}
				}
				addOrder(single, new OrderDataStore());
				for (auto *p : recJmpToEnd) {
					p->setJmpID(OrderDataList.size() - 1);
					p->setJmprefAdjusted();
				}

				break;
			}
			case KeywordID::LOOP: {
				KeywordNodeIDs.push(single->getIndex());
				const LoopStruct &loopstruct = parseToOrderLoop(*single);
				auto &doexps = loopstruct.getDoexps();
				size_t fid = OrderDataList.size();
				createOrderSub(doexps);
				OrderDataJump *p = new OrderDataJump();
				p->setJmpID(fid);
				p->setJmprefAdjusted();
				addOrder(p);
				addOrder(single, new OrderDataSingle(ObjectPtr(new Objects::Nil())));
				KeywordNodeIDs.pop();
				break;
			}
			case KeywordID::WHILE: {
				KeywordNodeIDs.push(single->getIndex());
				KeywordNodeIDs.pop();
				break;
			}
			}
		}
		void CreateOrder::setObjectIdentifier(ObjectPtr &op) {
			Objects::Identifier *ident = op.get<Objects::Identifier>();
			std::string name = ident->getName();
			size_t i;
			if ((i = DefFuncTable.find(name))) {
				op = ObjectPtr(new Objects::Function(i));
			}
			else if ((i = DefVariableTable.find(name))) {
				op = ObjectPtr(DefVariableTable[i].getData());
			}
			else if ((i = AddVariableTable.find(name))) {
				op = ObjectPtr(AddVariableTable[i].getData());
			}
			else {
				ident->setData(ObjectPtr(new Objects::Nil()));
				AddVariableTable.add(name, op);
				//error("Unfind Identifier(" + ident->getName() + ").");
			}
		}
	}
}
