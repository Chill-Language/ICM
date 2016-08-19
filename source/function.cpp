#include "function.h"
#include "tostring.h"
#include "typeobject.h"

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
			for (auto i : Range<size_t>(0, intype.size() - 1))
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
		void getAdjustedObjectPtr(ObjectPtr op, const TypeObject &type, DataList &dlp);
		bool Signature::checkType(const DataList &list, DataList &dlp) const {
			// Get TypeObject List
			vector<TypeObject> argT = getTypeObjectList(list);
			// Check Size
			if (last_is_args) {
				if (list.size() < InType.size())
					return false;
			}
			else if (list.size() != InType.size()) {
				return false;
			}
			// Check Type
			size_t size = last_is_args ? InType.size() - 1 : InType.size();
			Range<size_t> r1(0, size - 1);
			Range<size_t> r2(size, list.size() - 1);
			for (auto i : r1)
				if (!InType[i]->checkType(argT[i]))
					return false;
			if (last_is_args) {
				for (auto i : r2)
					if (!InType.back()->checkType(argT[i]))
						return false;
			}
			// Get Adjusted DataList
			dlp.clear();
			for (size_t i : r1)
				getAdjustedObjectPtr(list[i], *InType[i], dlp);
			if (last_is_args)
				for (size_t i : r2)
					getAdjustedObjectPtr(list[i], *InType.back(), dlp);
			return true;
		}
		vector<TypeObject> getTypeObjectList(const DataList &list)
		{
			vector<TypeObject> typelist;
			for (auto &e : list)
				typelist.push_back(getTypeObject(e));
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
	// Check Call
	ObjectPtr checkCall(const FuncTableUnit &ftu, const DataList &dl)
	{
		DataList ndl;
		size_t id = ftu.size();
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
		for (size_t i : Range<size_t>(0, ftu.size() - 1)) {
			if (ftu[i].checkType(nlist, ndl)) {
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
