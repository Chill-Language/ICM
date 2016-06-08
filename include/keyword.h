#ifndef _ICM_KEYWORD_H_
#define _ICM_KEYWORD_H_

#include <map>

namespace ICM
{
	using KeyWordMap = std::map<std::string, int>;
	using KeyWord = KeyWordMap::value_type;
}

#endif
