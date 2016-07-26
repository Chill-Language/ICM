#ifndef _ICM_FUNCTION_H_
#define _ICM_FUNCTION_H_

#include "ast.h"

namespace ICM
{
	namespace Objects
	{
		enum FuncParaType {
			FPT_Void,
			FPT_Fixed,
			FPT_Vary,
		};
		class FuncParameter
		{
		public:
			using TypeList = std::vector<DefaultType>;

			FuncParameter()
				: type(FPT_Void) {}

			FuncParameter(FuncParaType type, unsigned fixed_size, const TypeList &typelist)
				:  type(type), fixed_size(fixed_size) {
				switch (type) {
				case FPT_Void:
					this->fixed_size = 0;
					break;
				case FPT_Fixed:
					if (fixed_size == 0)
						this->type = FPT_Void;
					if (typelist.size() == fixed_size)
						this->typelist = typelist;
					else
						println("Unmatch Number for typelist in FuncParameter(Fixed).");
					break;
				case FPT_Vary:
					if (typelist.size() != fixed_size + 1)
						println("Unmatch Number for typelist in FuncParameter(Vary).");
					this->typelist = typelist;
					break;
				}
			}

			bool checkType(const DataList &list) const {
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
					return checkTypeList(list, fixed_size, list.size(), typelist.back());
				default:
					return false;
				}
			}

		private:
			bool checkTypeList(const DataList &list, unsigned size) const {
				for (auto i : Range<unsigned>(0, size - 1))
					if (list[i]->get_type() != typelist[i])
						return false;
				return true;
			}
			bool checkTypeList(const DataList &list, unsigned begindex, unsigned endindex, DefaultType vartype) const {
				for (auto i : Range<unsigned>(0, endindex - 1))
					if (list[i]->get_type() != vartype)
						return false;
				return true;
			}

		private:
			FuncParaType type = FPT_Void;
			unsigned fixed_size = 0;
			TypeList typelist;
		};
		class Function
		{
		public:
			Function(const std::string &name, const FuncParameter &fpars)
				: name(name), fpars(fpars) {}

		private:
			std::string name;
			FuncParameter fpars;
		};
	}
	ObjectPtr real_func(const DataList &dl, const FuncPtr& func, std::string name);
}

#endif
