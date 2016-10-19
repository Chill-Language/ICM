#include "function.h"
#include "typeobject.h"
#include "keyword.h"
#include "objectsdef.h"
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
		string Signature::to_string() const {
			std::string str;

			const auto &its = getInType();
			const auto &ots = getOutType();
			if (!its.empty()) {
				if (its.size() != 1) str.push_back('(');
				str.append(Convert::to_string(its.begin(), its.end(), [](const auto &e) { return e.to_string(); }));
				if (isLastArgs()) str.push_back('*');
				if (its.size() != 1) str.push_back(')');
			}
			else {
				str.append("Void");
			}
			str.append(" -> ");
			str.append(ots.to_string());

			return str;
		}
		vector<TypeObject> getTypeObjectList(const DataList &list);
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
		vector<TypeObject> getTypeObjectList(const DataList &list)
		{
			vector<TypeObject> typelist;
			for (auto &e : list)
				typelist.push_back(getTypeObject(e));
			return typelist;
		}
	}

	const Function::FuncObject* FuncTableUnit::checkType(const DataList &list, lightlist_creater<ObjectPtr> *dlp) const {
		Function::SignTreeMatch STM(ST);
		vector<TypeObject> typelist = Function::getTypeObjectList(list);
		const Function::FuncObject *ptr = STM.match(typelist);

		// Get Adjusted DataList
		if (ptr) {
			for (auto &v : list) {
				dlp->push_back(adjustObjectPtr(v));
			}
		}
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
			return checkSingle2(data, vec[index - 1].get(), type);
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
	const Function::FuncObject* Function::SignTreeMatch::match(const vector<TypeObject> &argT) {
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
		const vector<TypeObject> &typelist = Function::getTypeObjectList(dl);
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
		const vector<TypeObject> &typelist = Function::getTypeObjectList(dl);
#define USE_SIGNTREE false
#if USE_SIGNTREE
		const Function::FuncObject *p = ftu.checkType(nlist, &ndl);
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
			return p->call(ndl.data());
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
