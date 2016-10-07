#include "keyword.h"
#include "function.h"
#include "deffunc.h"
#include "objectsdef.h"
#include "number.h"


namespace ICM
{
	void addDefFuncs(FuncTable &DefFuncTable);
	// Create Default FuncTable
	void createDefFuncTable()
	{
		addDefFuncs(DefFuncTable);
		// TODO : Memory leak
		ObjectPtr nil(new Objects::Identifier("Nil", ObjectPtr(new Objects::Nil())));
		DefVariableTable.add("Nil", nil);
		//DefVariableTable.add("NIL", nil);
		//DefVariableTable.add("nil", nil);
		//ObjectPtr nan(new Objects::Identifier("NaN", ObjectPtr(new Objects::Number(Common::Number::Rational(0, 0)))));
		//DefVariableTable.add("NaN", nan);
	}

	BijectionKVMap<string, KeywordID> DefKeywordTable = {
		{ "let",      KeywordID::LET      },
		{ "cpy",      KeywordID::CPY      },
		{ "ref",      KeywordID::REF      },
		{ "if",       KeywordID::IF       },
		{ "then",     KeywordID::THEN     },
		{ "else",     KeywordID::ELSE     },
		{ "elsif",    KeywordID::ELSIF    },
		{ "loop",     KeywordID::LOOP     },
		{ "while",    KeywordID::WHILE    },
		{ "break",    KeywordID::BREAK    },
		{ "for",      KeywordID::FOR      },
		{ "in",       KeywordID::IN       },
		{ "to",       KeywordID::TO       },
		{ "case",     KeywordID::CASE     },
		{ "when",     KeywordID::WHEN     },
		{ "function", KeywordID::FUNCTION },
	};
}
