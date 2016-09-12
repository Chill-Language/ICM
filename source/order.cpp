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
			for (auto e : range(node.begin() + 1, node.end())) {
				doexps.push_back(e->get());
			}
			loopstruct.setDoexps(doexps);
		}

		//=======================================
		// * Order While
		//=======================================
		class WhileStruct
		{
			using BasePtr = AST::Base*;
			using DoExps = vector<BasePtr>;
		public:
			WhileStruct() {}
			void setDoexps(const DoExps &doexps) {
				this->doexps = doexps;
			}
			const DoExps& getDoexps() const {
				return doexps;
			}
			void setJudExp(const BasePtr &judexp) {
				this->judexp = judexp;
			}
			const BasePtr& getJudExp() const {
				return judexp;
			}

		private:
			BasePtr judexp;
			DoExps doexps;
		};
		void parseToOrderWhile(const AST::Node &node, WhileStruct &whilestruct);
		WhileStruct parseToOrderWhile(const AST::Node &node) {
			WhileStruct whilestruct;
			parseToOrderWhile(node, whilestruct);
			return whilestruct;
		}
		void parseToOrderWhile(const AST::Node &node, WhileStruct &whilestruct) {
			if (node.size() < 2) {
				error();
				return;
			}
			AST::Base* judexp = node[1].get();
			vector<AST::Base*> doexps;
			for (auto e : range(node.begin() + 2, node.end())) {
				doexps.push_back(e->get());
			}
			whilestruct.setJudExp(judexp);
			whilestruct.setDoexps(doexps);
		}

		//=======================================
		// * Order For
		//=======================================
		class ForStruct
		{
			using BasePtr = AST::Base*;
			using DoExps = vector<BasePtr>;
		public:
			ForStruct() {}
			void setObjVar(const ObjectPtr &objvar) {
				this->objvar = objvar;
			}
			const ObjectPtr& getObjVar() const {
				return objvar;
			}
			void setRanexp(const BasePtr &rangexp) {
				this->rangexp = rangexp;
			}
			const BasePtr& getRanexp() const {
				return rangexp;
			}
			void setDoexps(const DoExps &doexps) {
				this->doexps = doexps;
			}
			const DoExps& getDoexps() const {
				return doexps;
			}

		private:
			ObjectPtr objvar;
			BasePtr rangexp;
			DoExps doexps;
		};
		void parseToOrderFor(const AST::Node &node, ForStruct &forstruct);
		ForStruct parseToOrderFor(const AST::Node &node) {
			ForStruct forstruct;
			parseToOrderFor(node, forstruct);
			return forstruct;
		}
		void parseToOrderFor(const AST::Node &node, ForStruct &forstruct) {
			if (node.size() < 4) {
				error();
				return;
			}
			AST::Base *v = node[1].get();
			if (v->getType() != AST::Data::Type) {
				error();
				return;
			}
			forstruct.setObjVar(getNodeDataValue(v));
			if (!key_is(node[2].get(), KeywordID::IN)) {
				error();
				return;
			}
			forstruct.setRanexp(node[3].get());
			vector<AST::Base*> doexps;
			for (auto e : range(node.begin() + 4, node.end())) {
				doexps.push_back(e->get());
			}
			forstruct.setDoexps(doexps);
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
		void CreateOrder::createSingle(AST::Base *bp) {
			if (bp->getType() == AST::Refer::Type)
				createOrderSub(getReferNode(bp));
			else {
				ObjectPtr &op = getDataRef(bp);
				if (op.isType(T_Identifier))
					setObjectIdentifier(op);
				addOrder(new OrderDataSingle(op));
			}
		}

		// New
		vector<OrderData*>& CreateOrder::createOrder() {
			// Create Order
			addOrder(new OrderDataBegin());
			createOrderSub(getReferNode(Table[0]->front()));
			addOrder(Table[0].get(), new OrderDataEnd());
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
						createSingle(judexp);
						AST::Node *node = getReferNode(judexp);
						pjmprr = new OrderDataJumpNotIf(node->getIndex());
					}
					else { // AST::Data
						createSingle(judexp);
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
				const WhileStruct &whilestruct = parseToOrderWhile(*single);
				auto &judexp = whilestruct.getJudExp();
				auto &doexps = whilestruct.getDoexps();
				createSingle(judexp);
				size_t id = OrderDataList.size() - 1;
				OrderDataJumpNotIf *pjmprr = new OrderDataJumpNotIf(id);
				pjmprr->setExprefAdjusted();
				addOrder(pjmprr);
				createOrderSub(doexps);
				pjmprr->setJmpID(OrderDataList.size() + 1);
				pjmprr->setJmprefAdjusted();
				OrderDataJump *jmp = new OrderDataJump();
				jmp->setJmpID(id);
				jmp->setJmprefAdjusted();
				addOrder(jmp);
				addOrder(single, new OrderDataSingle(ObjectPtr(new Objects::Nil())));
				KeywordNodeIDs.pop();
				break;
			}
			case KeywordID::FOR: {
				KeywordNodeIDs.push(single->getIndex());
				const ForStruct &forstruct = parseToOrderFor(*single);
				ObjectPtr opvar = forstruct.getObjVar();
				if (opvar.isType(T_Identifier))
					setObjectIdentifier(opvar);
				else
					error();
				createSingle(forstruct.getRanexp());
				size_t atid = OrderDataList.size();
				addOrder(new OrderDataAt(atid - 1, 0));
				addOrder(new OrderDataAt(atid - 1, 1));
				addOrder(new OrderDataLet(opvar, atid));
				createOrderSub(forstruct.getDoexps());
				addOrder(new OrderDataEqu(opvar, atid + 1));
				OrderDataJumpNotIf *odjni = new OrderDataJumpNotIf(OrderDataList.size() - 1);
				odjni->setExprefAdjusted();
				odjni->setJmpID(atid + 2);
				odjni->setJmprefAdjusted();
				addOrder(new OrderDataInc(opvar));
				addOrder(odjni);
				addOrder(single, new OrderDataSingle(ObjectPtr(new Objects::Nil())));

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
