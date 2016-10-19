#ifndef _ICM_KEYWORD_H_
#define _ICM_KEYWORD_H_

#include "basic.h"

namespace ICM
{
	//=======================================
	// * Class Keyword
	//=======================================
	enum class KeywordID { PTI, LET, CPY, REF, IF, THEN, ELSE, ELSIF, FOR, IN, TO, WHILE, LOOP, BREAK, CASE, WHEN, FUNCTION };

	string to_string(KeywordID key);
}
#endif
