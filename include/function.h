#ifndef _ICM_FUNCTION_H_
#define _ICM_FUNCTION_H_

#include "ast.h"
#include "objects.h"

namespace ICM
{
	class FuncTableBase;
	enum FuncParaType {
		FPT_Void,
		FPT_Fixed,
		FPT_Vary,
		FPT_VaryIdentical,
	};
	class FuncParameter
	{
	public:
		using TypeList = std::vector<DefaultType>;

		FuncParameter()
			: type(FPT_Void) {}

		FuncParameter(FuncParaType type, unsigned fixed_size)
			: type(type), fixed_size(fixed_size) {
			initialize();
		}

		FuncParameter(FuncParaType type, unsigned fixed_size, const TypeList &typelist)
			: type(type), fixed_size(fixed_size), typelist(typelist) {
			initialize();
		}

		bool checkType(const DataList &list) const;


	private:
		void initialize();
		bool checkTypeList(const DataList &list, unsigned size) const;
		bool checkTypeList(const DataList &list, unsigned begindex, unsigned endindex) const;


	private:
		FuncParaType type = FPT_Void;
		unsigned fixed_size = 0;
		TypeList typelist;
	};
	ObjectPtr real_func(const DataList &dl, const FuncPtr& func, std::string name);
}

#endif
