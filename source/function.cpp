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
		Signature::Signature(const InitList1 &intype, const TypeObject& outtype, bool last_is_args)
			: InType(intype.size()), OutType(new TypeObject(outtype)), last_is_args(last_is_args) {
			auto p = intype.begin();
			for (auto i : Range<size_t>(0, intype.size()))
				InType[i] = shared_ptr<TypeObject>(new TypeObject(*p++));
		}
		string Signature::to_string() const {
			std::string str;

			const auto &its = getInType();
			const auto &ots = getOutType();
			if (!its.empty()) {
				if (its.size() != 1) str.push_back('(');
				str.append(Convert::to_string(its.begin(), its.end(), [](const auto &e) { return e->to_string(); }));
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
		vector<TypeObject> getTypeObjectList(const Signature::List &list);
		void getAdjustedObjectPtr(ObjectPtr op, const TypeObject &type, lightlist_creater<ObjectPtr> &dlp);
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
				if (!InType[i]->checkType(argT[i]))
					return false;
			if (last_is_args) {
				for (auto i : Range<size_t>(size, argT.size()))
					if (!InType.back()->checkType(argT[i]))
						return false;
			}
			return true;
		}
		bool Signature::checkType(const Signature &sign) const {
			// Get TypeObject List
			const vector<TypeObject> &argT = getTypeObjectList(sign.InType);
			// Check
			return checkType(argT);
		}
		bool Signature::checkType(const DataList &list, lightlist_creater<ObjectPtr> *dlp) const {
			// Get TypeObject List
			const vector<TypeObject> &argT = getTypeObjectList(list);
			// Check
			if (!checkType(argT))
				return false;
			// Get Adjusted DataList
			if (dlp) {
				size_t size = last_is_args ? InType.size() - 1 : InType.size();
				auto& dl = *dlp;
				dl.clear();
				for (size_t i : Range<size_t>(0, size))
					getAdjustedObjectPtr(list[i], *InType[i], dl);
				if (last_is_args)
					for (size_t i : Range<size_t>(size, argT.size()))
						getAdjustedObjectPtr(list[i], *InType.back(), dl);
			}
			return true;
		}
		vector<TypeObject> getTypeObjectList(const DataList &list)
		{
			vector<TypeObject> typelist;
			for (auto &e : list)
				typelist.push_back(getTypeObject(e));
			return typelist;
		}
		vector<TypeObject> getTypeObjectList(const Signature::List &list)
		{
			vector<TypeObject> typelist;
			for (auto &e : list)
				typelist.push_back(*e);
			return typelist;
		}
		void getAdjustedObjectPtr(ObjectPtr op, const TypeObject &type, lightlist_creater<ObjectPtr> &dlp)
		{
			//if (type.isIdent() && type.getValueType().isVary()) {
			//	dlp.push_back(op);
			//	return;
			//}
			if (!type.isVary())
				while (getTypeObject(op) != type)
					op = adjustObjectPtr(op);
			dlp.push_back(op);
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
		// Get Adjust DataList
		vector<ObjectPtr> nlist;
		for (auto &e : dl) {
			if (e.isType(T_Disperse)) {
				auto l = (Objects::Disperse*)(e.get());
				nlist.insert(nlist.end(), begin(l), end(l));
			}
			else
				nlist.push_back(e);
		}
		for (auto &e : nlist)
			if (e.isType(T_Identifier))
				e = adjustObjectPtr(e);

		lightlist_creater<ObjectPtr> ndl(nlist.size());
		size_t id = ftu.size();
		for (size_t i : Range<size_t>(0, ftu.size())) {
			if (ftu[i].checkType(dl, &ndl)) {
				id = i;
				break;
			}
		}
		return id;
	}
	// Check Call
	ObjectPtr checkCall(const FuncTableUnit &ftu, const DataList &dl)
	{
		// Get Adjust DataList
		vector<ObjectPtr> nlist;
		for (auto &e : dl) {
			if (e.isType(T_Disperse)) {
				auto l = (Objects::Disperse*)(e.get());
				nlist.insert(nlist.end(), begin(l), end(l));
			}
			else
				nlist.push_back(e);
		}
		for (auto &e : nlist)
			if (e.isType(T_Identifier))
				e = adjustObjectPtr(e);

		lightlist_creater<ObjectPtr> ndl(nlist.size());
		size_t id = ftu.size();
		for (size_t i : Range<size_t>(0, ftu.size())) {
			if (ftu[i].checkType(nlist, &ndl)) {
				id = i;
				break;
			}
		}
		//const Function::FuncObject *p = ftu.checkType(nlist, &ndl);
		CheckCallCount++;

		/*if (p != nullptr) {
			return p->call(ndl.data());
		}*/
		if (id != ftu.size()) {
			return ftu[id].call(ndl.data());
		}
		else {
			std::string errinfo = "Matching Types in function '" + ftu.getName() + "'.";
			return createError(errinfo);
		}
	}
}
