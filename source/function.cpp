#include "function.h"
#include "tostring.h"
#include "typeobject.h"
#include "keyword.h"

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
		void getAdjustedObjectPtr(ObjectPtr op, const TypeObject &type, DataList &dlp);
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
		bool Signature::checkType(const DataList &list, DataList *dlp) const {
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
		void getAdjustedObjectPtr(ObjectPtr op, const TypeObject &type, DataList &dlp)
		{
			if (type.isIdent() && type.getValueType().isVary()) {
				dlp.push_back(op);
				return;
			}
			if (!type.isVary())
				while (getTypeObject(op) != type)
					op = adjustObjectPtr(op);
			dlp.push_back(op);
		}
	}

	//=======================================
	// * Functions
	//=======================================
	// Get Call ID
	size_t getCallID(const FuncTableUnit &ftu, const DataList &dl)
	{
		// TODO
		DataList ndl;
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
		DataList nlist;
		for (auto &e : dl) {
			if (e->get_type() == T_Disperse) {
				auto l = (Objects::Disperse*)(e.get());
				nlist.insert(nlist.end(), begin(l), end(l));
			}
			else
				nlist.push_back(e);
		}

		DataList ndl;
		size_t id = ftu.size();
		for (size_t i : Range<size_t>(0, ftu.size())) {
			if (ftu[i].checkType(nlist, &ndl)) {
				id = i;
				break;
			}
		}

		if (id != ftu.size()) {
			return ftu[id].call(ndl);
		}
		else {
			std::string errinfo = "Matching Types in function '" + ftu.getName() + "'.";
			return createError(errinfo);
		}
	}
}
