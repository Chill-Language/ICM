#include "function.h"
#include "objectsdef.h"

namespace ICM
{
	//=======================================
	// * Class FuncParameter
	//=======================================
	void FuncParameter::initialize() {
		switch (type) {
		case FPT_Void:
			this->fixed_size = 0;
			break;
		case FPT_Fixed:
			if (fixed_size == 0)
				this->type = FPT_Void;
			if (typelist.size() != fixed_size)
				println("Unmatch Number for typelist in FuncParameter(Fixed).");
			break;
		case FPT_Vary:
			if (typelist.size() != fixed_size)
				println("Unmatch Number for typelist in FuncParameter(Vary).");
			break;
		case FPT_VaryIdentical:
			if (typelist.size() != fixed_size + 1)
				println("Unmatch Number for typelist in FuncParameter(VaryIdentical).");
			break;
		}
	}
	FuncParameter::DataListPtr FuncParameter::checkType(const DataList &list) const {
		switch (type) {
		case FPT_Void:
			return list.empty() ? getDataListPtr(list) : nullptr;
		case FPT_Fixed:
			if (list.size() != fixed_size)
				return nullptr;
			return checkTypeList(list, fixed_size);
		case FPT_Vary:
			if (list.size() < fixed_size)
				return nullptr;
			if (checkTypeList(list, fixed_size) == nullptr)
				return nullptr;
			return getDataListPtr(list);
		case FPT_VaryIdentical: {
			if (list.size() < fixed_size) return nullptr;
			DataListPtr p1 = checkTypeList(list, fixed_size);
			if (p1 == nullptr) return nullptr;
			DataListPtr p2 = checkTypeList(list, fixed_size, list.size());
			if (p2 == nullptr) return nullptr;
			DataListPtr dp(new DataList());
			for (auto &e : *p1) dp->push_back(e);
			for (auto &e : *p2) dp->push_back(e);
			return dp;
		}
		default:
			return nullptr;
		}
	}
	FuncParameter::DataListPtr FuncParameter::getDataListPtr(const DataList &list) const {
		DataListPtr dlp(new DataList());
		for (auto &e : list)
			dlp->push_back(e);
		return dlp;
	}
	ObjectPtr FuncParameter::checkSub(ObjectPtr ptr, DefaultType checktype) const {
		DefaultType currtype = ptr->get_type();
		if (currtype == T_Identifier && checktype != T_Identifier) {
			ptr = getPointer<Objects::Identifier>(ptr)->getValue()->getdata();
			currtype = ptr->get_type();
		}
		if (currtype != checktype && checktype != T_Vary)
			return nullptr;
		return ptr;
	}

	FuncParameter::DataListPtr FuncParameter::checkTypeList(const DataList &list, size_t size) const {
		DataListPtr dlp(new DataList());
		for (auto i : Range<size_t>(0, size - 1)) {
			ObjectPtr ptr = checkSub(list[i], typelist[i]);
			if (ptr == nullptr)
				return nullptr;
			dlp->push_back(ptr);
		}
		return dlp;
	}
	FuncParameter::DataListPtr FuncParameter::checkTypeList(const DataList &list, size_t bid, size_t eid) const {
		DataListPtr dlp(new DataList());
		if (list.empty())
			return dlp;
		DefaultType vartype = typelist.back();
		if (vartype == T_Vary) {
			ObjectPtr op = list[bid];
			vartype = op->get_type();
			if (vartype == T_Identifier)
				vartype = getPointer<Objects::Identifier>(op)->getValueType();
		}
		for (auto i : Range<size_t>(bid, eid - 1)) {
			ObjectPtr ptr = checkSub(list[i], vartype);
			if (ptr == nullptr)
				return nullptr;
			dlp->push_back(ptr);
		}
		return dlp;
	}

	//=======================================
	// * Function
	//=======================================
	// Check Call
	ObjectPtr checkCall(const FuncTableUnit &ftb, const DataList &dl)
	{
		auto p = ftb.getParameter().checkType(dl);
		if (p) {
			return ftb.getFuncPtr()(*p.get());
		}
		else {
			std::string errinfo = "Matching Types in function '" + ftb.getName() + "'.";
			return createError(errinfo);
		}
	}
}
