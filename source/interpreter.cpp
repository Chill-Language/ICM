#include "interpreter.h"
#include "objectsdef.h"

namespace ICM
{
	DataList Interpreter::createList(const Range<vector<shared_ptr<AST::Base>>::iterator> &r)
	{
		DataList listnum;
		for (const auto &l : r) {
			if ((*l)->getType() == AST::Data::Type) {
				auto &op = static_cast<AST::Data*>(l->get())->getData();
				if (op.isType(T_Identifier))
					listnum.push_back(op);
				else
					listnum.push_back(ObjectPtr(op->clone())); // TODO
			}
			else if ((*l)->getType() == AST::Refer::Type) {
				size_t id = static_cast<AST::Refer*>(l->get())->getData();
				listnum.push_back(tempresult[id]);
			}
		}
		return listnum;
	}
	void Interpreter::runFunc(const ObjectPtr &op, AST::Node *n, size_t id) {
		const auto &r = range(n->begin() + 1, n->end());
		const DataList &dl = createList(r);
		ObjectPtr &res = tempresult[id];

		if (op.isType(T_Function)) {
			const auto &ftu = op.get<Objects::Function>()->get_data();
			res = checkCall(ftu, dl);
		}
		else if (op.isType(T_Disperse)) {
			const auto &ftu = DefFuncTable["call"];
			DataList ndl = op.get<Objects::Disperse>()->getData();
			ndl.insert(ndl.end(), dl.begin(), dl.end());
			res = checkCall(ftu, ndl);
		}

		Result = res;
	}
	void Interpreter::runSub(const ObjectPtr &op, AST::Node *node, size_t i) {
		if (op.isType(T_Disperse) || op.isType(T_Function)) {
			runFunc(op, node, i);
		}
		else {
			println("Error Type.");
		}
	}
	ObjectPtr Interpreter::run() {
		using namespace ASTOrder;
		size_t ProgramCounter = 0;

		while (true) {
			auto &e = orderlist[ProgramCounter];
			switch (e->order()) {
			case OrderData::CALL: {
				AST::Node *node = static_cast<ASTOrder::OrderDataCall*>(e)->getData();
				AST::Base *f = node->front();
				if (f->getType() == AST::Data::Type) {
					AST::Data *nf = static_cast<AST::Data*>(f);
					const ObjectPtr &op = adjustObjectPtr(nf->getData());
					runSub(op, node, ProgramCounter);
				}
				else if (f->getType() == AST::Refer::Type) {
					AST::Refer *nf = static_cast<AST::Refer*>(f);
					const ObjectPtr &op = tempresult[nf->getData()];
					runSub(op, node, ProgramCounter);
				}
				break;
			}
			case OrderData::JUMP: {
				ASTOrder::OrderDataJump *p = static_cast<ASTOrder::OrderDataJump*>(e);
				size_t jmpid = p->getJmpid();
				ProgramCounter = jmpid;
				continue;
			}
			case OrderData::JUMPNOT: {
				ASTOrder::OrderDataJumpNotIf *p = static_cast<ASTOrder::OrderDataJumpNotIf*>(e);
				size_t expid = p->getExpid();
				ObjectPtr op = tempresult[expid];
				if (op.isType(T_Identifier)) {
					auto *pp = op.get<Objects::Identifier>();
					if (pp->getValueType() == T_Boolean) {
						op = pp->getRealData();
					}
					else {
						println("If exp with non Type Boolean.");
						break;
					}
				}
				if (op.isType(T_Boolean)) {
					bool result = op.get<Objects::Boolean>()->operator bool();
					if (!result) {
						size_t jmpid = p->getJmpid();
						ProgramCounter = jmpid;
						continue;
					}
				}
				else {
					println("If exp with non Type Boolean.");
				}
				break;
			}
			case OrderData::SINGLE: {
				tempresult[ProgramCounter] = static_cast<ASTOrder::OrderDataSingle*>(e)->getData();
				Result = tempresult[ProgramCounter];
				break;
			}
			case OrderData::STORE: {
				tempresult[ProgramCounter] = Result;
				break;
			}
			case OrderData::AT: {
				ASTOrder::OrderDataAt *p = static_cast<ASTOrder::OrderDataAt*>(e);
				Objects::List *l = tempresult[p->getRefid()].get<Objects::List>();
				ObjectPtr op = l->getData()[p->getIndex()];
				if (op.isType(T_Identifier))
					op = ObjectPtr(op.get<Objects::Identifier>()->getRealData());
				tempresult[ProgramCounter] = op;
				Result = tempresult[ProgramCounter];
				break;
			}
			case OrderData::LET: {
				ASTOrder::OrderDataLet *p = static_cast<ASTOrder::OrderDataLet*>(e);
				Objects::Identifier *ident = p->getData().get<Objects::Identifier>();
				ident->setData(tempresult[p->getRefid()]);
				break;
			}
			case OrderData::EQU: {
				ASTOrder::OrderDataEqu *p = static_cast<ASTOrder::OrderDataEqu*>(e);
				Objects::Identifier *ident = p->getData().get<Objects::Identifier>();
				Objects::Boolean *r = ident->getRealData()->equ(tempresult[p->getRefid()]);
				tempresult[ProgramCounter] = ObjectPtr(r);
				Result = tempresult[ProgramCounter];
				break;
			}
			case OrderData::INC: {
				ASTOrder::OrderDataInc *p = static_cast<ASTOrder::OrderDataInc*>(e);
				tempresult[ProgramCounter] = checkCall(DefFuncTable["inc"], DataList({ p->getData() }));
				Result = tempresult[ProgramCounter];
				break;
			}
			case OrderData::OVER: {
				return Result;
			}
			}
			ProgramCounter++;
		}

		return Result;
	}
}
