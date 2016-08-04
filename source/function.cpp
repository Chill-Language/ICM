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
	bool FuncParameter::checkType(const DataList &list, DataList &dlp) const {
		switch (type) {
		case FPT_Void:
			return list.empty();
		case FPT_Fixed:
			if (list.size() != fixed_size)
				return false;
			return checkTypeList(list, fixed_size, dlp);
		case FPT_Vary:
			if (list.size() < fixed_size)
				return false;
			if (!checkTypeList(list, fixed_size, dlp))
				return false;
			for (auto i : Range<size_t>(fixed_size, list.size() - 1))
				dlp.push_back(list[i]);
			return true;
		case FPT_VaryIdentical:
			if (list.size() < fixed_size)
				return false;
			if (!checkTypeList(list, fixed_size, dlp))
				return false;
			return checkTypeList(list, fixed_size, list.size(), dlp);
		default:
			return false;
		}
	}
	bool FuncParameter::checkTypeList(const DataList &list, size_t size, DataList &dlp) const {
		for (auto i : Range<size_t>(0, size - 1))
			if (!checkSub(list[i], typelist[i], dlp))
				return false;
		return true;
	}
	bool FuncParameter::checkTypeList(const DataList &list, size_t bid, size_t eid, DataList &dlp) const {
		if (list.empty())
			return true;
		DefaultType vartype = typelist.back();
		if (vartype == T_Vary) {
			ObjectPtr op = list[bid];
			vartype = op->get_type();
			if (vartype == T_Identifier)
				vartype = adjustObjectPtr(op)->get_type();
		}
		for (auto i : Range<size_t>(bid, eid - 1))
			if (!checkSub(list[i], vartype, dlp))
				return false;
		return true;
	}
	ObjectPtr FuncParameter::adjustObjectPtr(const ObjectPtr &ptr) const {
		if (ptr->get_type() == T_Identifier)
			return getPointer<Objects::Identifier>(ptr)->getRefNode()->getdata();
		else
			return ptr;
	}
	bool FuncParameter::checkSub(ObjectPtr ptr, DefaultType checktype, DataList &dlp) const {
		ptr = (checktype == T_Identifier) ? ptr : adjustObjectPtr(ptr);
		DefaultType currtype = ptr->get_type();
		if (currtype != checktype && checktype != T_Vary)
			return false;
		dlp.push_back(ptr);
		return true;
	}

	//=======================================
	// * Function
	//=======================================
	// Check Call
	ObjectPtr checkCall(const FuncTableUnit &ftb, const DataList &dl)
	{
		DataList ndl;
		if (ftb.getParameter().checkType(dl, ndl)) {
			//println(dl);
			//println(ndl);
			return ftb.getFuncPtr()(ndl);
		}
		else {
			std::string errinfo = "Matching Types in function '" + ftb.getName() + "'.";
			return createError(errinfo);
		}
	}
}
