#include "keyword.h"
#include "function.h"
#include "deffunc.h"
#include "objectsdef.h"
#include "number.h"

ICM::FuncTable DefFuncTable;
ICM::FuncTable AddFuncTable;
ICM::VariableTable DefVariableTable;
ICM::VariableTable AddVariableTable;

namespace ICM
{
	void addDefFuncs(FuncTable &DefFuncTable);
	// Create Default FuncTable
	void createDefFuncTable()
	{
		addDefFuncs(DefFuncTable);
		// TODO : Memory leak
		Objects::Identifier *nil = new Objects::Identifier("Nil", ObjectPtr(new Objects::Nil()));
		DefVariableTable.add("Nil", nil);
		DefVariableTable.add("NIL", nil);
		DefVariableTable.add("nil", nil);
		Objects::Identifier *nan = new Objects::Identifier("NaN", ObjectPtr(new Objects::Number(Common::Number::Rational(0, 0))));
		DefVariableTable.add("NaN", nan);
	}
}
