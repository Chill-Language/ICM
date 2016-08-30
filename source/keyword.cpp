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
	void createDefKeywordTable()
	{
		DefKetwordTable.add("if", KeywordID::IF);
		DefKetwordTable.add("then", KeywordID::THEN);
		DefKetwordTable.add("else", KeywordID::ELSE);
		DefKetwordTable.add("elsif", KeywordID::ELSIF);
		DefKetwordTable.add("while", KeywordID::WHILE);
		DefKetwordTable.add("break", KeywordID::BREAK);
		DefKetwordTable.add("for", KeywordID::FOR);
		DefKetwordTable.add("case", KeywordID::CASE);
		DefKetwordTable.add("function", KeywordID::FUNCTION);
	}
}
