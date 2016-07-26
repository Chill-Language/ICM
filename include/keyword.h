#ifndef _ICM_KEYWORD_H_
#define _ICM_KEYWORD_H_

#include "objects.h"

namespace ICM
{
	// KeyWord
	struct KeyWordBasic
	{
		KeyWordBasic(int id)
			: id(id) {}
		KeyWordBasic(int id, const FuncPtr& func) 
			: id(id), func(func) {}
		int id;
		FuncPtr func;
	};

	using KeyWordMap = std::map<std::string, KeyWordBasic>;
	using KeyWord = KeyWordMap::value_type;

	void createFuncMap();
	void createKeyWords();
}

#endif
