#include "keyword.h"
#include "function.h"

ICM::FuncTableType FuncTable;

namespace ICM
{

	FuncTableType::FuncTableType() {
		data.push_back(FuncTableBase(0, "", nullptr));
	}

	void FuncTableType::add(const string &name, const FuncPtr &func) {
		count++;
		data.push_back(FuncTableBase(count, name, func));
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

	// Create Defaulr FuncTable
	void createDefaultFuncTable()
	{
		using DLR = const DataList&;
		using namespace ICM::Objects;

		FuncTable.add("+", [](DLR dl) { return real_func(dl, Func::add, "+"); });
		FuncTable.add("-", [](DLR dl) { return real_func(dl, Func::sub, "-"); });
		FuncTable.add("*", [](DLR dl) { return real_func(dl, Func::mul, "*"); });
		FuncTable.add("/", [](DLR dl) { return real_func(dl, Func::div, "/"); });
		FuncTable.add("print", [](DLR dl) { Func::print(dl.at(0)); return dl.at(0); });
	}
}
