#include "interpreter.h"
#include "objectsdef.h"

namespace ICM
{
	DataList Interpreter::createList(const RangeIterator<vector<AST::Element>::iterator> &r)
	{
		lightlist_creater<ObjectPtr> listnum(r.size());
		for (const auto &l : r) {
			if (l.isData()) {
				ObjectPtr op(l.getData());
				if (op.isType(T_Identifier))
					listnum.push_back(op);
				else
					listnum.push_back(ObjectPtr(op->clone())); // TODO
			}
			else {
				size_t id = l.getRefer();
				listnum.push_back(tempresult[id]);
			}
		}
		return listnum.data();
	}
	void Interpreter::runFunc(const ObjectPtr &op, AST::Node *n, size_t id) {
		const auto &r = rangei(n->begin() + 1, n->end());
		const DataList &dl = createList(r);
		ObjectPtr &res = tempresult[id];

		if (op.isType(T_Function)) {
			const auto &ftu = op.get<Objects::Function>()->getData().get_data();
			res = checkCall(ftu, dl);
		}
		else if (op.isType(T_Disperse)) {
			const auto &ftu = DefFuncTable["call"];
			auto ndl = op.get<Objects::Disperse>()->getData().getData();
			ndl.insert(ndl.end(), dl.begin(), dl.end());
			res = checkCall(ftu, DataList(ndl));
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
	ObjectPtr Interpreter::getObjectPtr(AST::Element *e) {
		if (e->isData())
			return ObjectPtr(e->getData());
		else
			return tempresult[e->getRefer()];
	}
	DataList Interpreter::getDataList(const vector<AST::Element*> &vb) {
		lightlist_creater<ObjectPtr> ndl(vb.size());
		for (auto &e : vb)
			ndl.push_back(getObjectPtr(e));
		return ndl.data();
	}
	ObjectPtr Interpreter::run() {
		using namespace ASTOrder;
		size_t ProgramCounter = 0;
		//vector<size_t> VecPC;
		while (true) {
			//VecPC.push_back(ProgramCounter);
			auto &e = orderlist[ProgramCounter];
			//println("[", ProgramCounter, "] ", e->to_string());
			switch (e->order()) {
			case OrderData::CCAL: {
				AST::Node *node = static_cast<ASTOrder::OrderDataCheckCall*>(e)->getData();
				AST::Element *f = &node->front();
				if (f->isData()) {
					const ObjectPtr &op = adjustObjectPtr(ObjectPtr(f->getData()));
					runSub(op, node, ProgramCounter);
				}
				else {
					const ObjectPtr &op = tempresult[f->getRefer()];
					runSub(op, node, ProgramCounter);
				}
				break;
			}
			case OrderData::CALL: {
				ASTOrder::OrderDataCall *p = static_cast<ASTOrder::OrderDataCall*>(e);
				DataList dl = getDataList(p->getData());
				
				tempresult[ProgramCounter] = p->call(dl);
				Result = tempresult[ProgramCounter];
				break;
			}
			case OrderData::CHKT: {
				ASTOrder::OrderDataCheckType *p = static_cast<ASTOrder::OrderDataCheckType*>(e);
				if (!adjustObjectPtr(getObjectPtr(p->getData())).isType(p->getType())) {
					println("Error in Check Type.", p->to_string());
					return ObjectPtr();
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
					bool result = op.get<Objects::Boolean>()->getData();
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
				const ObjectPtr &op = static_cast<ASTOrder::OrderDataSingle*>(e)->getData();
				if (op.isType(T_Identifier))
					tempresult[ProgramCounter] = static_cast<ASTOrder::OrderDataSingle*>(e)->getData();
				else
					tempresult[ProgramCounter] = ObjectPtr(op->clone());
				Result = tempresult[ProgramCounter];
				break;
			}
			case OrderData::STORE: {
				tempresult[ProgramCounter] = Result;
				break;
			}
			case OrderData::AT: {
				ASTOrder::OrderDataAt *p = static_cast<ASTOrder::OrderDataAt*>(e);
				ObjectPtr top = tempresult[p->getRefid()];
				if (top.isType(T_Identifier))
					top = top.get<Objects::Identifier>()->getRealData();
				Objects::List *l = top.get<Objects::List>();
				ObjectPtr op = l->getData().getData()[p->getIndex()];
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
				
				tempresult[ProgramCounter] = p->getData();
				Result = tempresult[ProgramCounter];
				break;
			}
			case OrderData::CPY: {
				ASTOrder::OrderDataCpy *p = static_cast<ASTOrder::OrderDataCpy*>(e);
				Objects::Identifier *ident = p->getData().get<Objects::Identifier>();
				ident->setCopy(tempresult[p->getRefid()]);

				tempresult[ProgramCounter] = p->getData();
				Result = tempresult[ProgramCounter];
				break;
			}
			case OrderData::REF: {
				ASTOrder::OrderDataRef *p = static_cast<ASTOrder::OrderDataRef*>(e);
				Objects::Identifier *ident = p->getData().get<Objects::Identifier>();
				ident->setRefer(tempresult[p->getRefid()]);

				tempresult[ProgramCounter] = p->getData();
				Result = tempresult[ProgramCounter];
				break;
			}
			case OrderData::CPYS: {
				ASTOrder::OrderDataCpySingle *p = static_cast<ASTOrder::OrderDataCpySingle*>(e);
				size_t id = p->getRefid();
				Objects::Identifier temp;
				temp.setCopy(tempresult[id]);

				tempresult[ProgramCounter] = temp.getData();
				Result = tempresult[ProgramCounter];
				break;
			}
			case OrderData::REFS: {
				ASTOrder::OrderDataCpySingle *p = static_cast<ASTOrder::OrderDataCpySingle*>(e);
				size_t id = p->getRefid();
				// TODO
				tempresult[ProgramCounter] = ObjectPtr(tempresult[id]);
				Result = tempresult[ProgramCounter];
				break;
			}
			case OrderData::EQU: {
				ASTOrder::OrderDataEqu *p = static_cast<ASTOrder::OrderDataEqu*>(e);
				Objects::Identifier *ident = p->getData().get<Objects::Identifier>();
				bool r = ident->getRealData()->equ(tempresult[p->getRefid()]);
				tempresult[ProgramCounter] = ObjectPtr(new Objects::Boolean(r));
				Result = tempresult[ProgramCounter];
				break;
			}
			case OrderData::SML: {
				ASTOrder::OrderDataSmall *p = static_cast<ASTOrder::OrderDataSmall*>(e);
				Objects::Identifier *ident = p->getData().get<Objects::Identifier>();
				tempresult[ProgramCounter] = checkCall(DefFuncTable["<"], DataList({ ident->getRealData(), tempresult[p->getRefid()] }));
				Result = tempresult[ProgramCounter];
				break;
			}
			case OrderData::SME: {
				ASTOrder::OrderDataSmallEqual *p = static_cast<ASTOrder::OrderDataSmallEqual*>(e);
				Objects::Identifier *ident = p->getData().get<Objects::Identifier>();
				const ObjectPtr &op = checkCall(DefFuncTable["<="], DataList({ ident->getRealData(), tempresult[p->getRefid()] }));
				tempresult[ProgramCounter] = op;
				Result = tempresult[ProgramCounter];
				break;
			}
			case OrderData::LAR: {
				ASTOrder::OrderDataSmall *p = static_cast<ASTOrder::OrderDataSmall*>(e);
				Objects::Identifier *ident = p->getData().get<Objects::Identifier>();
				tempresult[ProgramCounter] = checkCall(DefFuncTable[">"], DataList({ ident->getRealData(), tempresult[p->getRefid()] }));
				Result = tempresult[ProgramCounter];
				break;
			}
			case OrderData::LAE: {
				ASTOrder::OrderDataSmallEqual *p = static_cast<ASTOrder::OrderDataSmallEqual*>(e);
				Objects::Identifier *ident = p->getData().get<Objects::Identifier>();
				const ObjectPtr &op = checkCall(DefFuncTable[">="], DataList({ ident->getRealData(), tempresult[p->getRefid()] }));
				tempresult[ProgramCounter] = op;
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
				//for (auto &e : VecPC)
				//	print(e, "->");
				return Result;
			}
			}
			ProgramCounter++;
		}

		return Result;
	}
}
