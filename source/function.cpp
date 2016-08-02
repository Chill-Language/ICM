#include "function.h"
#include "keyword.h"

namespace ICM
{
	void FuncParameter::initialize()
	{
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
	bool FuncParameter::checkType(const DataList &list) const {
		switch (type) {
		case FPT_Void:
			return list.empty();
		case FPT_Fixed:
			if (list.size() != fixed_size)
				return false;
			return checkTypeList(list, fixed_size);
		case FPT_Vary:
			if (list.size() < fixed_size)
				return false;
			if (!checkTypeList(list, fixed_size))
				return false;
			return true;
		case FPT_VaryIdentical:
			if (list.size() < fixed_size)
				return false;
			if (!checkTypeList(list, fixed_size))
				return false;
			return checkTypeList(list, fixed_size, list.size());
		default:
			return false;
		}
	}
	bool FuncParameter::checkTypeList(const DataList &list, unsigned size) const {
		for (auto i : Range<unsigned>(0, size - 1))
			if (list[i]->get_type() != typelist[i] && typelist[i] != T_Vary)
				return false;
		return true;
	}
	bool FuncParameter::checkTypeList(const DataList &list, unsigned begindex, unsigned endindex) const {
		DefaultType vartype = typelist.back();
		if (list.empty())
			return true;
		if (vartype == T_Vary)
			vartype = list[0]->get_type();
		for (auto i : Range<unsigned>(0, endindex - 1))
			if (list[i]->get_type() != vartype && vartype != T_Vary)
				return false;
		return true;
	}

	// Check Call
	ObjectPtr checkCall(const FuncTableBase &ftb, const DataList &dl)
	{
		if (ftb.getParameter().checkType(dl)) {
			return ftb.getFuncPtr()(dl);
		}
		else {
			std::string errinfo = "Matching Types in function '" + ftb.getName() + "'.";
			return ObjectPtr(new Objects::Error(errinfo));
		}
	}
}
