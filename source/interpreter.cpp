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
			DataList ndl = op.get<Objects::Disperse>()->get_data();
			ndl.insert(ndl.end(), dl.begin(), dl.end());
			res = checkCall(ftu, ndl);
		}
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
		ObjectPtr result;
		for (size_t i : range(0, orderlist.size()-1)) {
			auto &e = orderlist[i];
			//println(e->to_string());
			//println(e->getData()->to_string());
			AST::Node *node = e->getData();
			AST::Base *f = node->front();
			if (f->getType() == AST::Data::Type) {
				AST::Data *nf = static_cast<AST::Data*>(f);
				const ObjectPtr &op = adjustObjectPtr(nf->getData());
				runSub(op, node, i);
			}
			else if (f->getType() == AST::Refer::Type) {
				AST::Refer *nf = static_cast<AST::Refer*>(f);
				const ObjectPtr &op = tempresult[nf->getData()];
				runSub(op, node, i);
			}
		}
		// TODO
		result = tempresult[tempresult.size()-2];
		return result;
	}
}
