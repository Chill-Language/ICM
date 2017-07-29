#include "basic.h"
#include "runtime/function.h"
#include "runtime/typeobject.h"
#include "parser/keyword.h"
#include "runtime/objectdef.h"
#include <ctime>
size_t CheckCallCount = 0;

namespace ICM
{
	//=======================================
	// * Namespace Function
	//=======================================
	namespace Function
	{
		//=======================================
		// * Class Signature
		//=======================================
		Signature::Signature(const InitList &intype, const TypeObject& outtype, bool last_is_args)
			: InType(intype), OutType(outtype), last_is_args(last_is_args) {}
		lightlist<TypeObject> getTypeObjectList(const DataList &list);
		bool Signature::checkType(const lightlist<TypeObject> &argT) const {
			// Check Size
			if (last_is_args) {
				if (argT.size() < InType.size())
					return false;
			}
			else if (argT.size() != InType.size()) {
				return false;
			}
			// Check Type
			size_t size = last_is_args ? InType.size() - 1 : InType.size();
			for (auto i : Range<size_t>(0, size))
				if (!InType[i].checkType(argT[i]))
					return false;
			if (last_is_args) {
				for (auto i : Range<size_t>(size, argT.size()))
					if (!InType.back().checkType(argT[i]))
						return false;
			}
			return true;
		}
		bool Signature::checkType(const vector<TypeObject> &argT) const {
			// Check Size
			if (last_is_args) {
				if (argT.size() < InType.size())
					return false;
			}
			else if (argT.size() != InType.size()) {
				return false;
			}
			// Check Type
			size_t size = last_is_args ? InType.size() - 1 : InType.size();
			for (auto i : Range<size_t>(0, size))
				if (!InType[i].checkType(argT[i]))
					return false;
			if (last_is_args) {
				for (auto i : Range<size_t>(size, argT.size()))
					if (!InType.back().checkType(argT[i]))
						return false;
			}
			return true;
		}
		bool Signature::checkType(const Signature &sign) const {
			return checkType(sign.InType);
		}
		inline ICM::TypeObject getTypeObject(const ObjectPtr &op)
		{
			if (op.isType(T_Function)) {
				TypeObject t(T_Function);
				auto &ft = op->get<T_Function>()->getData();
				t.setFuncTableUnit(&ft);
				return t;
			}
			else
				return ICM::TypeObject(op.type());
		}
		lightlist<TypeObject> getTypeObjectList(const DataList &list)
		{
			lightlist<TypeObject> typelist(list.size());
			std::transform(_PointerIterator(list.begin()), _PointerIterator(list.end()), _PointerIterator(typelist.begin()), getTypeObject);
			return typelist;
		}
	}

	const Function::FuncObject* FuncTableUnit::checkType(const lightlist<TypeObject> &typelist) const {
		Function::SignTreeMatch STM(ST);
		const Function::FuncObject *ptr = STM.match(typelist);

		return ptr;
	}

	//=======================================

	const Function::SignTreeMatch::Node* Function::SignTreeMatch::checkSingle(const Node *data, const TypeObject &type, size_t &index) {
		auto &vec = data->getChildren();
		if (index == 0) {
			for (size_t i : range(0, vec.size())) {
				bool change = false;
				auto *p = checkSingle(data, vec[i].get(), type, change);
				if (p != nullptr) {
					if (change) {
						index = i + 1;
					}
					return p;
				}
			}
		}
		else {
			const Node *p = vec[index - 1].get();
			if (!p->isNull()) {
				if (p->checkType(type)) {
					return data;
				}
			}
			return nullptr;
		}
		return nullptr;
	}
	const Function::SignTreeMatch::Node* Function::SignTreeMatch::checkSingle(const Node *data, const Node *p, const TypeObject &type, bool &change) {
		if (!p->isNull()) {
			bool r;
			if (p->isFunc())
				r = (p->getSign().checkType(type.getSign()));
			else
				r = (p->checkType(type));
			if (r) {
				if (p->isArgs()) {
					change = true;
					return data;
				}
				else
					return p;
			}
		}
		return nullptr;
	}
	const Function::FuncObject* Function::SignTreeMatch::match(const lightlist<TypeObject> &argT) {
		const Node* currptr = ST.getRoot();
		size_t index = 0;
		for (auto &t : argT) {
			currptr = checkSingle(currptr, t, index);
			if (currptr == nullptr)
				return nullptr;
		}
		if (index)
			currptr = currptr->getChildren()[index - 1].get();
		size_t callindex;
		bool r = currptr->canEnd(callindex);
		if (r)
			return ST.getFunc(callindex);
		else
			return nullptr;
	}

	//=======================================
	// * Functions
	//=======================================
	// Get Call ID
	size_t getCallID(const FuncTableUnit &ftu, const DataList &dl)
	{
		size_t id = ftu.size();
		const lightlist<TypeObject> &typelist = Function::getTypeObjectList(dl);
		for (size_t i : Range<size_t>(0, ftu.size())) {
			if (ftu[i].checkType(typelist)) {
				id = i;
				break;
			}
		}
		return id;
	}
	// Check Call
	ObjectPtr checkCall(const FuncTableUnit &ftu, const DataList &dl)
	{
		const lightlist<TypeObject> &typelist = Function::getTypeObjectList(dl);
#define USE_SIGNTREE false
#if USE_SIGNTREE
		const Function::FuncObject *p = ftu.checkType(typelist);
#else
		size_t id = ftu.size();
		for (size_t i : Range<size_t>(0, ftu.size())) {
			if (ftu[i].checkType(typelist)) {
				id = i;
				break;
			}
		}
#endif
		CheckCallCount++;
#if USE_SIGNTREE
		if (p != nullptr) {
			return p->call(dl);
		}
#else
		if (id != ftu.size()) {
			return ftu[id].call(dl);
		}
#endif
		else {
			std::string errinfo = "Matching Types in function '" + ftu.getName() + "'.";
			return createError(errinfo);
		}
	}
}
