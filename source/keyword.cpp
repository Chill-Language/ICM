#include "keyword.h"
#include "function.h"

std::vector<ICM::FuncPtr> FuncMap;
ICM::KeyWordMap KeyWords;

namespace ICM
{
	void createFuncMap()
	{
		using DLR = const DataList&;
		using namespace ICM::Objects;
		
		FuncMap = {
			[](DLR dl) { return createObject(T_Object); },
			[](DLR dl) { return real_func(dl, Func::add, "+"); },
			[](DLR dl) { return real_func(dl, Func::sub, "-"); },
			[](DLR dl) { return real_func(dl, Func::mul, "*"); },
			[](DLR dl) { return real_func(dl, Func::div, "/"); },
			[](DLR dl) { Func::print(dl.at(0)); return createObject(T_Object); },
		};
	}
	void createKeyWords()
	{
		KeyWords = {
			KeyWord("+",{ 1, FuncMap[1] }),
			KeyWord("-",{ 2, FuncMap[2] }),
			KeyWord("*",{ 3, FuncMap[3] }),
			KeyWord("/",{ 4, FuncMap[4] }),
			KeyWord("%%",{ 211 }),
			KeyWord("=",{ 10 }),
			KeyWord(">",{ 11 }),
			KeyWord("<",{ 12 }),
			KeyWord(">=",{ 13 }),
			KeyWord("<=",{ 14 }),
			KeyWord("list",{ 20 }),
			KeyWord("print",{ 5, FuncMap[5] }),
		};
	}
}
