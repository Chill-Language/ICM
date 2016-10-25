#include "order.h"
#include "objectdef.h"
#include "config.h"

namespace ICM
{
	namespace ASTOrder
	{
		//=======================================
		// * Functions
		//=======================================
		bool key_is(const AST::Element &node, KeywordID key)
		{
			using Key = Objects::Keyword;
			if (!node.isData())
				return false;
			ObjectPtr op = node.getData();
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
			using BasePtr = AST::Element*;
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
			AST::Element* judexp = (AST::Element*)&node[range.begin()];
			vector<AST::Element*> doexps;
			enum { IF, ELSE, ELSEIF } mode = IF;
			bool startelse = false;
			size_t begin = (key_is(node[2], KeywordID::THEN)) ? *range.begin() + 2 : *range.begin() + 1;
			for (size_t i : Range<size_t>(begin, range.end())) {
				AST::Element &e = (AST::Element&)node[i];
				switch (mode)
				{
				case IF:
					if (e.isRefer()) {
						doexps.push_back(&e);
					}
					else if (key_is(e, KeywordID::ELSE)) {
						mode = ELSE;
						startelse = true;
					}
					else if (key_is(e, KeywordID::ELSIF)) {
						mode = ELSEIF;
					}
					else {
						doexps.push_back(&e);
					}
					break;
				case ELSE:
					if (e.isRefer()) {
						ifstruct.add_elexp(&e);
					}
					else if (key_is(e, KeywordID::IF)) {
						mode = ELSEIF;
					}
					else {
						ifstruct.add_elexp(&e);
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
			using BasePtr = AST::Element*;
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
			vector<AST::Element*> doexps;
			for (auto &e : rangei(node.begin() + 1, node.end())) {
				doexps.push_back((AST::Element*)&e);
			}
			loopstruct.setDoexps(doexps);
		}

		//=======================================
		// * Order While
		//=======================================
		class WhileStruct
		{
			using BasePtr = AST::Element*;
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
			AST::Element* judexp = (AST::Element*)&node[1];
			vector<AST::Element*> doexps;
			for (auto &e : rangei(node.begin() + 2, node.end())) {
				doexps.push_back((AST::Element*)&e);
			}
			whilestruct.setJudExp(judexp);
			whilestruct.setDoexps(doexps);
		}

		//=======================================
		// * Order For
		//=======================================
		class ForStruct
		{
			using BasePtr = AST::Element*;
			using DoExps = vector<BasePtr>;
		public:
			ForStruct() {}
			void setObjVar(const ObjectPtr &objvar) {
				this->objvar = objvar;
			}
			const ObjectPtr& getObjVar() const {
				return objvar;
			}
			void setRanexpBegin(const BasePtr &rangexp) {
				this->rangexpb = rangexp;
			}
			const BasePtr& getRanexpBegin() const {
				return rangexpb;
			}
			void setRanexpEnd(const BasePtr &rangexp) {
				this->rangexpe = rangexp;
			}
			const BasePtr& getRanexpEnd() const {
				return rangexpe;
			}
			void setDoexps(const DoExps &doexps) {
				this->doexps = doexps;
			}
			const DoExps& getDoexps() const {
				return doexps;
			}

		private:
			ObjectPtr objvar;
			BasePtr rangexpb;
			BasePtr rangexpe;
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
			AST::Element *v = (AST::Element*)&node[1];
			if (!v->isData()) {
				error();
				return;
			}
			forstruct.setObjVar(v->getData());
			if (!key_is(node[2], KeywordID::IN)) {
				error();
				return;
			}
			forstruct.setRanexpBegin((AST::Element*)&node[3]);
			if (!key_is(node[4], KeywordID::TO)) {
				error();
				return;
			}
			forstruct.setRanexpEnd((AST::Element*)&node[5]);
			vector<AST::Element*> doexps;
			for (auto &e : rangei(node.begin() + 6, node.end())) {
				doexps.push_back((AST::Element*)&e);
			}
			forstruct.setDoexps(doexps);
		}


		//=======================================
		// * Create Order
		//=======================================
		AST::Node* CreateOrder::getReferNode(AST::Element &refer) {
			return Table[refer.getRefer()].get();
		}
		ObjectPtr& CreateOrder::getDataRef(AST::Element &data) {
			return data.getData();
		}
		void CreateOrder::createSingle(AST::Element &bp) {
			if (bp.isRefer())
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
				println(to_string_code(*single));
			AST::Element *front = (AST::Element*)&(single->front());
			if (front->isData()) {
				ObjectPtr &op = getDataRef(*front);
				if (op.isType<Objects::Keyword>()) {
					createOrderKeyword(single, op.get<Objects::Keyword>()->getData());
					return;
				}
			}

			for (auto &p : *single) {
				if (p.isData()) {
					ObjectPtr &op = getDataRef(p);
					if (op.isType<Objects::Identifier>())
						setObjectIdentifier(op);
				}
				else {
					createOrderSub(getReferNode(p));
				}
			}
			addOrder(single, new ASTOrder::OrderDataCheckCall(single));
		}
		void CreateOrder::createOrderSub(const Segment &segment) {
			for (AST::Element* single : segment) {

				if (single->isData()) {
					ObjectPtr &op = getDataRef(*single);
					if (op.isType(T_Keyword)) {
						createOrderKeywordSingle(op.get<Objects::Keyword>()->getData());
						continue;
					}
					if (op.isType(T_Identifier))
						setObjectIdentifier(op);
					addOrder(new OrderDataSingle(op));
				}
				else {
					createOrderSub(getReferNode(*single));
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
					if (judexp->isRefer()) {
						createSingle(*judexp);
						AST::Node *node = getReferNode(*judexp);
						pjmprr = new OrderDataJumpNotIf(node->getIndex());
					}
					else {
						createSingle(*judexp);
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
				createSingle(*judexp);
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
				createSingle(*forstruct.getRanexpBegin());
				size_t bid = OrderDataList.size() - 1;
				createSingle(*forstruct.getRanexpEnd());
				size_t eid = OrderDataList.size() - 1;
				addOrder(new OrderDataLet(opvar, bid));
				createOrderSub(forstruct.getDoexps());
				addOrder(new OrderDataLargeEqual(opvar, eid));
				OrderDataJumpNotIf *odjni = new OrderDataJumpNotIf(OrderDataList.size() - 1);
				odjni->setExprefAdjusted();
				odjni->setJmpID(eid + 2);
				odjni->setJmprefAdjusted();
				addOrder(new OrderDataInc(opvar));
				addOrder(odjni);
				addOrder(single, new OrderDataSingle(ObjectPtr(new Objects::Nil())));

				KeywordNodeIDs.pop();
				break;
			}
			case KeywordID::PTI: {
				vector<size_t> indexs;
				for (auto &n : rangei(single->begin() + 1, single->end())) {
					createSingle(n);
					indexs.push_back(OrderDataList.size() - 1);
				}
				addOrder(single, new OrderDataPrintIdent(indexs));
				break;
			}
			case KeywordID::LET:
			case KeywordID::CPY:
			case KeywordID::REF: {
				if (single->size() < 2) {
					error();
					break;
				}

				if (single->size() == 2) {
					if (keyword == KeywordID::CPY) {
						createSingle((*single)[1]);
						addOrder(single, new OrderDataCpySingle(OrderDataList.size() - 1));
					}
					else if (keyword == KeywordID::REF) {
						createSingle((*single)[1]);
						addOrder(single, new OrderDataRefSingle(OrderDataList.size() - 1));
					}
					break;
				}

				if (single->size() != 3) {
					error();
					break;
				}


				if (!(*single)[1].isData()) {
					error();
					break;
				}
				ObjectPtr &op = getDataRef((*single)[1]);
				if (!op.isType(T_Identifier)) {
					error();
					break;
				}

				Objects::Identifier *ident = op.get<Objects::Identifier>();
				string name = ident->getData().getName();
				size_t id;
				if (id = AddVariableTable.find(name)) {
					ident = AddVariableTable[id].getData();
				}
				else {
					AddVariableTable.add(name, op);
				}

				op = ObjectPtr(ident);

				createSingle((*single)[2]);
				switch (keyword) {
				case KeywordID::LET:
					addOrder(single, new OrderDataLet(op, OrderDataList.size() - 1));
					break;
				case KeywordID::CPY:
					addOrder(single, new OrderDataCpy(op, OrderDataList.size() - 1));
					break;
				case KeywordID::REF:
					addOrder(single, new OrderDataRef(op, OrderDataList.size() - 1));
					break;
				}
				break;
			}
			}
		}
		void CreateOrder::setObjectIdentifier(ObjectPtr &op) {
			Objects::Identifier *ident = op.get<Objects::Identifier>();
			std::string name = ident->getData().getName();
			size_t i;
			if ((i = DefFuncTable.find(name))) {
				op = ObjectPtr(new Objects::Function(TypeBase::FunctionType(i)));
			}
			else if ((i = DefVariableTable.find(name))) {
				op = ObjectPtr(DefVariableTable[i].getData());
			}
			else if ((i = AddVariableTable.find(name))) {
				op = ObjectPtr(AddVariableTable[i].getData());
			}
			else {
				ident->getData().setData(ObjectPtr(new Objects::Nil()));
				AddVariableTable.add(name, op);
				//error("Unfind Identifier(" + ident->getName() + ").");
			}
		}
	}
}
