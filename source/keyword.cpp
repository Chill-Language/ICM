#include "keyword.h"
#include "function.h"

ICM::FuncTableType FuncTable;

namespace ICM
{

	FuncTableType::FuncTableType() {
		data.push_back(FuncTableBase(0, "", nullptr, FuncParameter()));
	}

	void FuncTableType::add(const string &name, const FuncPtr &func, const FuncParameter &pars) {
		count++;
		data.push_back(FuncTableBase(count, name, func, pars));
		keymap[name] = count;
	}
	const FuncTableBase& FuncTableType::operator[](size_t id) const {
		return data[id];
	}
	size_t FuncTableType::find(const string &name) const {
		auto iter = keymap.find(name);
		if (iter != keymap.end())
			return iter->second;
		return 0;
	}

	// Create Default FuncTable
	void createDefaultFuncTable()
	{
		using namespace ICM::Objects::Func;

		FuncTable.add("+", add, FuncParameter(FPT_VaryIdentical, 0, { T_Vary }));
		FuncTable.add("-", sub, FuncParameter(FPT_VaryIdentical, 0, { T_Number }));
		FuncTable.add("*", mul, FuncParameter(FPT_VaryIdentical, 0, { T_Number }));
		FuncTable.add("/", div, FuncParameter(FPT_VaryIdentical, 0, { T_Number }));
		FuncTable.add("%", mod, FuncParameter(FPT_VaryIdentical, 0, { T_Number }));
		FuncTable.add("=", equ, FuncParameter(FPT_Vary, 0));
		FuncTable.add("print", print, FuncParameter(FPT_Vary, 0));
		FuncTable.add("list", list, FuncParameter(FPT_Vary, 0));
		//FuncTable.add("let", let, FuncParameter(FPT_VaryIdentical, 0, { T_List }));
	}
}
