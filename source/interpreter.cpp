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
				listnum.push_back(op);
			}
			else if ((*l)->getType() == AST::Refer::Type) {
				size_t id = static_cast<AST::Refer*>(l->get())->getData();
				listnum.push_back(tempresult[id]);
			}
		}
		return listnum;
	}
	void Interpreter::runFunc(const ObjectPtr &op, AST::Node *n) {
		auto &ftu = op.get<Objects::Function>()->get_data();
		const auto &r = range(n->begin() + 1, n->end());
		const auto &res = checkCall(ftu, createList(r));
		tempresult.push_back(res);
	}
	ObjectPtr Interpreter::run() {
		ObjectPtr result;
		for (auto &e : orderlist) {
			AST::Node *n = e->getData();
			AST::Base *f = n->front();
			if (f->getType() == AST::Data::Type) {
				auto dat = static_cast<AST::Data*>(f);
				ObjectPtr &op = dat->getData();
				if (op->get_type() == T_Function) {
					runFunc(op, n);
				}
				else if (op->get_type() == T_Identifier) {
					const ObjectPtr &nop = op.get<Objects::Identifier>()->getData();
					if (nop->get_type() == T_Function) {
						runFunc(nop, n);
					}
					else if (nop->get_type() == T_Identifier) {
						const ObjectPtr &nnop = nop.get<Objects::Identifier>()->getData();
						if (nnop->get_type() == T_Function) {
							runFunc(nnop, n);
						}
					}
				}
			}
			else if (f->getType() == AST::Refer::Type) {
				size_t id = static_cast<AST::Refer*>(f)->getData();
				result = tempresult[id];
				//println(result->to_string());
				//println(to_string(result->get_type()));
				if (result.isType(T_Disperse)) {
					const DataList &dl = result.get<Objects::Disperse>()->get_data();
					if (dl.front().isType(T_Function)) {
						// TODO
						auto &ftu = dl.front().get<Objects::Function>()->get_data();
						const auto &r = range(n->begin() + 1, n->end());
						DataList dd = createList(r);
						dd.insert(dd.end(), dl.begin() + 1, dl.end());
						const auto &res = checkCall(ftu, dd);
						tempresult.push_back(res);
					}
				}
				if (result.isType(T_Function))
					runFunc(result, n);
				else {
					//println("Error : " + result->to_output() + "is not function.");
					tempresult.push_back(result);
				}
			}
		}
		return result;
	}
}
