#include "function.h"

namespace ICM
{
	// TEMP
	ObjectPtr real_func(const DataList &dl, const FuncPtr& func, std::string name)
	{
		using namespace Objects;
		DefaultType type = dl.front()->get_type();
		FuncParameter func_add_pars(FPT_Vary, 0, { type });
		if (func_add_pars.checkType(dl)) {
			return func(dl);
		}
		else {
			println("Error for Matching Types in function '" + name + "'.");
		}
		return createObject(T_Object, "");
	}
}
