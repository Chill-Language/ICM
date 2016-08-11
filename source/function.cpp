#include "function.h"
#include "objectsdef.h"

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
		bool Signature::checkType(const DataList &list, DataList &dlp) const {
			// Check Size
			if (last_is_args) {
				if (list.size() < InType.size())
					return false;
			}
			else if (list.size() != InType.size()) {
				return false;
			}
			// Check Type
			dlp.clear();
			size_t size = last_is_args ? InType.size() - 1 : InType.size();
			for (auto i : Range<size_t>(0, size - 1))
				if (!checkSub(list[i], InType[i], dlp))
					return false;
			if (last_is_args) {
				for (auto i : Range<size_t>(size, list.size() - 1))
					if (!checkSub(list[i], InType.back(), dlp))
						return false;
			}
			return true;
		}
		bool Signature::checkSub(ObjectPtr ptr, DefaultType checktype, DataList &dlp) const {
			ptr = (checktype == T_Identifier) ? ptr : adjustObjectPtr(ptr);
			DefaultType currtype = ptr->get_type();
			if (currtype != checktype && checktype != T_Vary)
				return false;
			dlp.push_back(ptr);
			return true;
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
		for (size_t i : Range<size_t>(0, ftu.size() - 1)) {
			if (ftu[i].checkType(dl, ndl)) {
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
