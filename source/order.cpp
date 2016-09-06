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

		void parserToOrderIf(const AST::Node &node, const Range<size_t> &range, IfStruct &ifstruct);

		IfStruct parserToOrderIf(const AST::Node &node)
		{
			IfStruct ifstruct;
			parserToOrderIf(node, Range<size_t>(1, node.size()), ifstruct);
			return ifstruct;
		}

		void parserToOrderIf(const AST::Node &node, const Range<size_t> &range, IfStruct &ifstruct)
		{
			if (range.size() < 2)
				error();
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
					parserToOrderIf(node, Range<size_t>(i, node.size()), ifstruct);
					return;
				}
			}
			if (startelse)
				error();
			ifstruct.add(judexp, doexps);
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
			addOrder(Table[0].get(), new OrderDataRet(getReferNode(Table[0]->front())->getIndex()));
			// Adjust Refer
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
		void CreateOrder::createOrderKeyword(const Single& single, KeywordID keyword) {
			switch (keyword) {
			case KeywordID::IF: {
				const IfStruct &ifstruct = parserToOrderIf(*single);


				break;
			}
			case KeywordID::WHILE: {
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


		////////////////////////////////////////////////
		// Old
		/*void CreateOrder::createOrderKeyword(AST::Node &node, KeywordID keyword)
		{
			switch (keyword)
			{
			case ICM::KeywordID::IF: {
				const IfStruct &ifstruct = parserToOrderIf(node);
				auto &v = ifstruct.getIfexps();
				////
				size_t i = 0;
				vector<OrderData*> vec;


				switch (v[i].judexp->getType()) {
				case AST::Data::Type: {
					auto e = v[i].judexp;
					ObjectPtr &op = getNodeDataRef(e);
					//vec.push_back(new OrderDataJumpIfV(op,2));
					GlobalOrderDataList.push_back(new OrderDataJumpNotIfVR(op, GlobalOrderDataList.size()+1));
					break;
				}
				case AST::Refer::Type: {
					addReferMap(node);
					size_t id = static_cast<AST::Refer*>(v[i].judexp)->getData();
					createOrderASToneNode(*Table[id]);
					//vec.push_back(new OrderDataJumpIf(id, 2));
					//createOrderASToneNode(static_cast<AST::Node&>(*v[i].doexps.front()));
					GlobalOrderDataList.push_back(new OrderDataJumpNotIfRR(id, GlobalOrderDataList.size()+1));
					break;
				}
				};
				//ObjectPtr temp;
				//auto j1 = const_cast<AST::Node*>(static_cast<const AST::Node*>(v[0].judexp));
				//vec.push_back(new OrderDataCall(temp, j1));
				//vec.push_back(new OrderDataRet(5));
				//vec.push_back(new OrderDataEnd());

				for (auto *e : vec)
					println(e->to_string());
				////
				//for (size_t i : range(0, v.size())) {
				//}
				break;
			}
			case ICM::KeywordID::FOR:
				break;
			case ICM::KeywordID::WHILE:
				break;
			case ICM::KeywordID::LOOP:
				break;
			case ICM::KeywordID::CASE:
				break;
			case ICM::KeywordID::FUNCTION:
				break;
			default:
				break;
			}
		}*/
	}
}
