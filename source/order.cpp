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
			size_t begin = (key_is(node[1].get(), KeywordID::THEN)) ? *range.begin() + 2 : *range.begin() + 1;
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
		vector<OrderDataCall*> CreateOrder::createOrderASTs()
		{
			AST::Node &node = *table[0];
			createOrderASToneNode(node);
			if (GlobalConfig.PrintOrder) {
				println();
				for (size_t i : range(0, GlobalOrderDataList.size()))
					println(GlobalOrderDataList[i]->to_string());
				println();
			}
			return GlobalOrderDataList;
		}
		void CreateOrder::createOrderASToneNode(AST::Node &node)
		{
			if (GlobalConfig.PrintOrder)
				println(node.to_string_code());
			for (auto &e : node)
			{
				if (e->getType() == AST::Data::Type) {
					ObjectPtr &op = getNodeDataRef(e.get());

					if (op.isType<Objects::Keyword>()) {
						createOrderKeyword(node, op.get<Objects::Keyword>()->getData());
					}
					else if (op.isType<Objects::Identifier>()) {
						Objects::Identifier *ident = op.get<Objects::Identifier>();
						size_t i;
						if ((i = DefFuncTable.find(ident->getName()))) {
							op = ObjectPtr(new Objects::Function(i));
						}
						else if ((i = DefVariableTable.find(ident->getName()))) {
							op = ObjectPtr(DefVariableTable[i].getData());
						}
						else if ((i = AddVariableTable.find(ident->getName()))) {
							op = ObjectPtr(AddVariableTable[i].getData());
						}
						else {
							ident->setData(ObjectPtr(new Objects::Nil()));
							AddVariableTable.add(ident->getName(), ObjectPtr(ident));
							//error("Unfind Identifier(" + ident->getName() + ").");
						}
						//createOrderIdentifier(node, getPointer<Objects::Identifier>(op));
					}
				}
				else if (e->getType() == AST::Refer::Type) {
					AST::Refer *ref = static_cast<AST::Refer*>(e.get());
					size_t id = ref->getData();
					createOrderASToneNode(*table[id]);
					ref->setData(GlobalOrderDataListReferMap[id]);
				}
				else {
					error();
				}
			}
			size_t id = GlobalOrderDataListReferMap.size();
			// TODO : Change Map to Vector.
#ifdef _WIN32
			GlobalOrderDataListReferMap[node.getIndex()] = id;
#else
			GlobalOrderDataListReferMap[node.getIndex()] = id - 1;
#endif
			GlobalOrderDataList.push_back(new ASTOrder::OrderDataCall((AST::Node*)&node));
		}
		void CreateOrder::createOrderKeyword(AST::Node &node, KeywordID keyword)
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
					auto e = v[0].judexp;
					ObjectPtr &op = getNodeDataRef(e);
					vec.push_back(new OrderDataJumpIf(op));
					break;
				}
				case AST::Refer::Type:;
				};
				//ObjectPtr temp;
				//auto j1 = const_cast<AST::Node*>(static_cast<const AST::Node*>(v[0].judexp));
				//vec.push_back(new OrderDataCall(temp, j1));
				vec.push_back(new OrderDataRet());
				vec.push_back(new OrderDataEnd());

				for (auto *e : vec)
					println(e->to_string());
				////
				for (size_t i : range(0, v.size())) {
				}
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
		}
		void CreateOrder::createOrderIdentifier(AST::Node &node, Objects::Identifier *ident)
		{
		}
	}
}
