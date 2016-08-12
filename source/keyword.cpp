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
		// TODO
		Objects::Identifier *idp = new Objects::Identifier("Nil", new ASTNode(ObjectPtr(new Objects::Nil())));
		DefVariableTable.add("Nil", idp);
		DefVariableTable.add("NIL", idp);
		Objects::Identifier *nan = new Objects::Identifier("NaN", new ASTNode(ObjectPtr(new Objects::Number(Common::Number::Rational(0, 0)))));
		DefVariableTable.add("NaN", nan);
	}
}
